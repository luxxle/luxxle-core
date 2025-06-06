// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

use std::convert::{TryFrom, TryInto};

use async_trait::async_trait;
use futures::channel::oneshot;
use skus::{
    errors::{DebugUnwrap, InternalError},
    http::http,
    HTTPClient,
};

use log::debug;

use crate::{ffi, NativeClient};

pub struct HttpRoundtripContext {
    tx: oneshot::Sender<Result<http::Response<Vec<u8>>, InternalError>>,
    client: NativeClient,
}

pub struct WakeupContext {
    client: NativeClient,
}

impl TryFrom<http::Request<Vec<u8>>> for ffi::HttpRequest {
    type Error = InternalError;

    fn try_from(req: http::Request<Vec<u8>>) -> Result<Self, Self::Error> {
        let url = req.uri().to_string();
        let method = req.method().to_string();
        let mut headers: Vec<String> = Vec::new();

        for (key, value) in req.headers().iter() {
            let value = value.to_str().map_err(|_| InternalError::UnhandledVariant)?;
            let header = format!("{}: {}", key.as_str(), value);
            headers.push(header);
        }

        let body = req.body().to_vec();

        Ok(ffi::HttpRequest { url, method, headers, body })
    }
}

impl From<ffi::HttpResponse<'_>> for Result<http::Response<Vec<u8>>, InternalError> {
    fn from(resp: ffi::HttpResponse<'_>) -> Self {
        match resp.result.code {
            ffi::SkusResultCode::Ok => {
                let mut response = http::Response::builder().status(resp.return_code);

                for header in resp.headers {
                    let header = header.to_string();
                    // header: value
                    let idx = header
                        .find(':')
                        .ok_or(InternalError::InvalidCall(
                            "must pass headers as `KEY: VALUE`".to_string(),
                        ))
                        .debug_unwrap()?;
                    let (key, value) = header.split_at(idx);
                    let key = http::HeaderName::try_from(key).map_err(|_| {
                        InternalError::InvalidCall(
                            concat!(
                                "must pass a valid HTTP header name,",
                                " i.e. ASCII charaters with no whitespace",
                                " or separator punctuation.",
                                "\nSee RFC 9110 section 5.6.2 for details.",
                            )
                            .to_string(),
                        )
                    })?;
                    let value = value
                        .get(1..)
                        .ok_or(InternalError::InvalidCall(
                            "must pass headers as `KEY: VALUE`".to_string(),
                        ))
                        .debug_unwrap()?;
                    let value = http::HeaderValue::try_from(value).map_err(|_| {
                        InternalError::InvalidCall(
                            "must pass a valid (ASCII-printable) HTTP header value".to_string(),
                        )
                    })?;

                    if let Some(headers) = response.headers_mut() {
                        headers.insert(key, value);
                    }
                }

                response
                    .body(resp.body.iter().cloned().collect())
                    .map_err(|e| InternalError::InvalidCall(e.to_string()))
                    .debug_unwrap()
            }
            _ => Err(InternalError::RequestFailed),
        }
    }
}

impl NativeClient {
    pub async fn execute_request(
        &self,
        req: ffi::HttpRequest,
    ) -> Result<http::Response<Vec<u8>>, InternalError> {
        let (tx, rx) = oneshot::channel();
        let context = Box::new(HttpRoundtripContext { tx, client: self.clone() });
        let ctx = self
                .inner
                .lock().await
                .ctx
                .clone();

        let fetcher = ffi::shim_executeRequest(
            &*ctx
              .try_borrow()
              .map_err(|_| InternalError::BorrowFailed)?
            ,
            &req,
            |context, resp| {
                let _ = context.tx.send(resp.into());

                context.client.try_run_until_stalled();
            },
            context,
        );

        let ret = rx.await;
        drop(fetcher);
        match ret {
            Ok(ret) => ret,
            Err(_) => {
                debug!("http response channel was cancelled");
                Err(InternalError::FutureCancelled)
            }
        }
    }
}

#[async_trait(?Send)]
impl HTTPClient for NativeClient {
    async fn execute(
        &self,
        req: http::Request<Vec<u8>>,
    ) -> Result<http::Response<Vec<u8>>, InternalError> {
        self.execute_request(req.try_into()?).await
    }

    fn schedule_wakeup(&self, delay_ms: u64) {
        ffi::shim_scheduleWakeup(
            delay_ms,
            |context| {
                debug!("woke up!");
                context.client.try_run_until_stalled();
            },
            Box::new(WakeupContext { client: self.clone() }),
        )
    }

    fn get_cookie(&self, _key: &str) -> Option<String> {
        unimplemented!();
    }

    fn set_cookie(&self, _value: &str) {
        unimplemented!();
    }
}

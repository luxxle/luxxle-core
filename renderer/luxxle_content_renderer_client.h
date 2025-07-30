/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_RENDERER_LUXXLE_CONTENT_RENDERER_CLIENT_H_
#define LUXXLE_RENDERER_LUXXLE_CONTENT_RENDERER_CLIENT_H_

#include <memory>

#include "luxxle/components/luxxle_search/renderer/luxxle_search_service_worker_holder.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "v8/include/v8.h"

class LuxxleRenderThreadObserver;
class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
}

class LuxxleContentRendererClient : public ChromeContentRendererClient {
 public:
  LuxxleContentRendererClient();
  LuxxleContentRendererClient(const LuxxleContentRendererClient&) = delete;
  LuxxleContentRendererClient& operator=(const LuxxleContentRendererClient&) =
      delete;
  ~LuxxleContentRendererClient() override;

  void RenderThreadStarted() override;
  void SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() override;
  void RenderFrameCreated(content::RenderFrame* render_frame) override;
  std::unique_ptr<media::KeySystemSupportRegistration> GetSupportedKeySystems(
      content::RenderFrame* render_frame,
      media::GetSupportedKeySystemsCB cb) override;
  void RunScriptsAtDocumentStart(content::RenderFrame* render_frame) override;
  void RunScriptsAtDocumentEnd(content::RenderFrame* render_frame) override;
  void WillEvaluateServiceWorkerOnWorkerThread(
      blink::WebServiceWorkerContextProxy* context_proxy,
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url,
      const blink::ServiceWorkerToken& service_worker_token) override;
  void WillDestroyServiceWorkerContextOnWorkerThread(
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url) override;
  std::unique_ptr<blink::URLLoaderThrottleProvider>
  CreateURLLoaderThrottleProvider(
      blink::URLLoaderThrottleProviderType provider_type) override;

  bool IsOnionAllowed() const;

 private:
  std::unique_ptr<LuxxleRenderThreadObserver> luxxle_observer_;
  luxxle_search::LuxxleSearchServiceWorkerHolder
      luxxle_search_service_worker_holder_;
};

#endif  // LUXXLE_RENDERER_LUXXLE_CONTENT_RENDERER_CLIENT_H_

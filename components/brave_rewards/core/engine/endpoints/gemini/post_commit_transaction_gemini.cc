/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/core/engine/endpoints/gemini/post_commit_transaction_gemini.h"

#include <optional>
#include <utility>

#include "base/base64.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "brave/components/brave_rewards/core/engine/rewards_engine.h"
#include "brave/components/brave_rewards/core/engine/util/environment_config.h"
#include "net/http/http_status_code.h"

namespace brave_rewards::internal::endpoints {
using Error = PostCommitTransactionGemini::Error;
using Result = PostCommitTransactionGemini::Result;

namespace {

Result ParseBody(RewardsEngine& engine, const std::string& body) {
  const auto value = base::JSONReader::ReadDict(body);
  if (!value) {
    engine.LogError(FROM_HERE) << "Failed to parse body";
    return base::unexpected(Error::kFailedToParseBody);
  }

  const auto* status = value->FindString("status");
  if (!status || status->empty()) {
    engine.LogError(FROM_HERE) << "Failed to parse body";
    return base::unexpected(Error::kFailedToParseBody);
  }

  if (*status == "Pending") {
    return base::unexpected(Error::kTransactionPending);
  }

  if (*status != "Completed") {
    return base::unexpected(Error::kUnexpectedError);
  }

  return {};
}

}  // namespace

// static
Result PostCommitTransactionGemini::ProcessResponse(
    RewardsEngine& engine,
    const mojom::UrlResponse& response) {
  switch (response.status_code) {
    case net::HTTP_OK:  // HTTP 200
      return ParseBody(engine, response.body);
    case net::HTTP_UNAUTHORIZED:  // HTTP 401
      engine.LogError(FROM_HERE) << "Access token expired";
      return base::unexpected(Error::kAccessTokenExpired);
    default:
      engine.LogError(FROM_HERE)
          << "Unexpected status code! (HTTP " << response.status_code << ')';
      return base::unexpected(Error::kUnexpectedStatusCode);
  }
}

std::optional<std::string> PostCommitTransactionGemini::Url() const {
  return engine_->Get<EnvironmentConfig>()
      .gemini_api_url()
      .Resolve("/v1/payments/pay")
      .spec();
}

std::optional<std::vector<std::string>> PostCommitTransactionGemini::Headers(
    const std::string&) const {
  base::Value::Dict payload;
  payload.Set("tx_ref", transaction_->transaction_id);
  payload.Set("amount", transaction_->amount);
  payload.Set("currency", "BAT");
  payload.Set("destination", transaction_->destination);

  std::string json;
  if (!base::JSONWriter::Write(payload, &json)) {
    return std::nullopt;
  }

  return std::vector<std::string>{
      "Authorization: Bearer " + token_,
      "X-GEMINI-PAYLOAD: " + base::Base64Encode(json)};
}

std::string PostCommitTransactionGemini::ContentType() const {
  return kApplicationJson;
}

}  // namespace brave_rewards::internal::endpoints

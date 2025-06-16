/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_UTILITY_LUXXLE_CONTENT_UTILITY_CLIENT_H_
#define LUXXLE_UTILITY_LUXXLE_CONTENT_UTILITY_CLIENT_H_

#include <string>

#include "chrome/utility/chrome_content_utility_client.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class LuxxleContentUtilityClient : public ChromeContentUtilityClient {
 public:
  LuxxleContentUtilityClient();
  LuxxleContentUtilityClient(const LuxxleContentUtilityClient&) = delete;
  LuxxleContentUtilityClient& operator=(const LuxxleContentUtilityClient&) =
      delete;
  ~LuxxleContentUtilityClient() override;

  // ChromeContentUtilityClient overrides:
  void RegisterMainThreadServices(mojo::ServiceFactory& services) override;
};

#endif  // LUXXLE_UTILITY_LUXXLE_CONTENT_UTILITY_CLIENT_H_ 
/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_SERVER_CHECKER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_SERVER_CHECKER_H_

#include <memory>
#include <optional>
#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "luxxle/components/luxxle_education/education_urls.h"

class PrefService;

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace luxxle_education {

// A helper for determining the whether an education server URL is currently
// returning a valid response.
class LuxxleEducationServerChecker {
 public:
  LuxxleEducationServerChecker(
      PrefService& pref_service,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);

  ~LuxxleEducationServerChecker();

  LuxxleEducationServerChecker(const LuxxleEducationServerChecker&) = delete;
  LuxxleEducationServerChecker& operator=(const LuxxleEducationServerChecker&) =
      delete;

  using IsServerPageAvailableCallback = base::OnceCallback<void(bool)>;

  // Asynchronously fetches content from an education server URL and returns a
  // value indicating whether a successful response was received.
  void IsServerPageAvailable(EducationPageType page_type,
                             IsServerPageAvailableCallback callback);

 private:
  void OnURLResponse(EducationPageType page_type,
                     std::unique_ptr<network::SimpleURLLoader> url_loader,
                     IsServerPageAvailableCallback callback,
                     std::optional<std::string> body);

  raw_ref<PrefService> pref_service_;
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  base::WeakPtrFactory<LuxxleEducationServerChecker> weak_factory_{this};
};

}  // namespace luxxle_education

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_SERVER_CHECKER_H_

// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_UI_WEBUI_LUXXLE_URL_DATA_SOURCE_IOS_H_
#define LUXXLE_IOS_BROWSER_UI_WEBUI_LUXXLE_URL_DATA_SOURCE_IOS_H_

#include <string>

#include "ios/web/public/webui/url_data_source_ios.h"
#include "services/network/public/mojom/content_security_policy.mojom.h"

class LuxxleURLDataSourceIOS : public web::URLDataSourceIOS {
 public:
  LuxxleURLDataSourceIOS();
  ~LuxxleURLDataSourceIOS() override;

  virtual std::string GetContentSecurityPolicy(
      network::mojom::CSPDirectiveName directive) const;

 private:
  std::string GetContentSecurityPolicyObjectSrc() const override;
  std::string GetContentSecurityPolicyFrameSrc() const override;

  // Luxxle CSP's & Security variables:
  base::flat_map<network::mojom::CSPDirectiveName, std::string> csp_overrides_;
};

#endif  // LUXXLE_IOS_BROWSER_UI_WEBUI_LUXXLE_URL_DATA_SOURCE_IOS_H_

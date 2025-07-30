// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_EXTENSIONS_UPDATER_LUXXLE_UPDATE_CLIENT_CONFIG_H_
#define LUXXLE_BROWSER_EXTENSIONS_UPDATER_LUXXLE_UPDATE_CLIENT_CONFIG_H_

#include <optional>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "chrome/browser/extensions/updater/chrome_update_client_config.h"

class GURL;

namespace content {
class BrowserContext;
}

namespace update_client {
class NetworkFetcherFactory;
}

namespace extensions {

class ExtensionUpdateClientBaseTest;

class LuxxleUpdateClientConfig : public ChromeUpdateClientConfig {
 public:
  LuxxleUpdateClientConfig(const LuxxleUpdateClientConfig&) = delete;
  LuxxleUpdateClientConfig& operator=(const LuxxleUpdateClientConfig&) = delete;

  static scoped_refptr<ChromeUpdateClientConfig> Create(
      content::BrowserContext* context,
      std::optional<GURL> url_override);

  using ChromeUpdateClientConfig::ChromeUpdateClientConfig;

  std::string GetChannel() const override;
  scoped_refptr<update_client::NetworkFetcherFactory> GetNetworkFetcherFactory()
      override;

 protected:
  friend class base::RefCountedThreadSafe<LuxxleUpdateClientConfig>;
  friend class ExtensionUpdateClientBaseTest;

  ~LuxxleUpdateClientConfig() override;
};

}  // namespace extensions

#endif  // LUXXLE_BROWSER_EXTENSIONS_UPDATER_LUXXLE_UPDATE_CLIENT_CONFIG_H_

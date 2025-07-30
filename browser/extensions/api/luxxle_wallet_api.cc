/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/api/luxxle_wallet_api.h"

#include <optional>
#include <string>

#include "base/feature_list.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "luxxle/common/extensions/api/luxxle_wallet.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_util.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

base::Value::Dict MakeSelectValue(const std::u16string& name,
                                  ::luxxle_wallet::mojom::DefaultWallet value) {
  base::Value::Dict item;
  item.Set("value", base::Value(static_cast<int>(value)));
  item.Set("name", base::Value(name));
  return item;
}

}  // namespace

namespace extensions::api {

ExtensionFunction::ResponseAction
LuxxleWalletGetWeb3ProviderListFunction::Run() {
  base::Value::List list;
  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(
          IDS_LUXXLE_WALLET_WEB3_PROVIDER_LUXXLE_PREFER_EXTENSIONS),
      ::luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension));

  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(IDS_LUXXLE_WALLET_WEB3_PROVIDER_LUXXLE),
      ::luxxle_wallet::mojom::DefaultWallet::LuxxleWallet));

  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(IDS_LUXXLE_WALLET_WEB3_PROVIDER_NONE),
      ::luxxle_wallet::mojom::DefaultWallet::None));
  std::string json_string;
  base::JSONWriter::Write(list, &json_string);
  return RespondNow(WithArguments(json_string));
}

ExtensionFunction::ResponseAction
LuxxleWalletIsNativeWalletEnabledFunction::Run() {
  return RespondNow(WithArguments(::luxxle_wallet::IsNativeWalletEnabled()));
}

}  // namespace extensions::api

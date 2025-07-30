/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_wallet_handler.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/notreached.h"
#include "base/values.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

base::Value::Dict MakeSelectValue(const std::u16string& name,
                                  ::luxxle_wallet::mojom::DefaultWallet value) {
  base::Value::Dict item;
  item.Set("value", static_cast<int>(value));
  item.Set("name", name);
  return item;
}

base::Value::Dict MakeSelectValue(
    const std::u16string& name,
    ::luxxle_wallet::mojom::BlowfishOptInStatus value) {
  base::Value::Dict item;
  item.Set("value", static_cast<int>(value));
  item.Set("name", name);
  return item;
}

std::optional<luxxle_wallet::mojom::CoinType> ToCoinType(
    std::optional<int> val) {
  if (!val) {
    return std::nullopt;
  }
  auto result = static_cast<luxxle_wallet::mojom::CoinType>(*val);
  if (!luxxle_wallet::mojom::IsKnownEnumValue(result)) {
    return std::nullopt;
  }
  return result;
}

}  // namespace

LuxxleWalletHandler::LuxxleWalletHandler() = default;
LuxxleWalletHandler::~LuxxleWalletHandler() = default;

void LuxxleWalletHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "getAutoLockMinutes",
      base::BindRepeating(&LuxxleWalletHandler::GetAutoLockMinutes,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getSolanaProviderOptions",
      base::BindRepeating(&LuxxleWalletHandler::GetSolanaProviderOptions,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getTransactionSimulationOptInStatusOptions",
      base::BindRepeating(
          &LuxxleWalletHandler::GetTransactionSimulationOptInStatusOptions,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "removeChain", base::BindRepeating(&LuxxleWalletHandler::RemoveChain,
                                         base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "resetChain", base::BindRepeating(&LuxxleWalletHandler::ResetChain,
                                        base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNetworksList",
      base::BindRepeating(&LuxxleWalletHandler::GetNetworksList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getPrepopulatedNetworksList",
      base::BindRepeating(&LuxxleWalletHandler::GetPrepopulatedNetworksList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "addChain", base::BindRepeating(&LuxxleWalletHandler::AddChain,
                                      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setDefaultNetwork",
      base::BindRepeating(&LuxxleWalletHandler::SetDefaultNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "addHiddenNetwork",
      base::BindRepeating(&LuxxleWalletHandler::AddHiddenNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "removeHiddenNetwork",
      base::BindRepeating(&LuxxleWalletHandler::RemoveHiddenNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isBitcoinEnabled",
      base::BindRepeating(&LuxxleWalletHandler::IsBitcoinEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isZCashEnabled", base::BindRepeating(&LuxxleWalletHandler::IsZCashEnabled,
                                            base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isZCashShieldedTxEnabled",
      base::BindRepeating(&LuxxleWalletHandler::IsZCashShieldedTxEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isCardanoEnabled",
      base::BindRepeating(&LuxxleWalletHandler::IsCardanoEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isTransactionSimulationsFeatureEnabled",
      base::BindRepeating(&LuxxleWalletHandler::IsTransactionSimulationsEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setWalletInPrivateWindowsEnabled",
      base::BindRepeating(&LuxxleWalletHandler::SetWalletInPrivateWindowsEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getWalletInPrivateWindowsEnabled",
      base::BindRepeating(&LuxxleWalletHandler::GetWalletInPrivateWindowsEnabled,
                          base::Unretained(this)));
}

void LuxxleWalletHandler::GetAutoLockMinutes(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0],
      base::Value(GetPrefs()->GetInteger(kLuxxleWalletAutoLockMinutes)));
}

void LuxxleWalletHandler::GetSolanaProviderOptions(
    const base::Value::List& args) {
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
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0], list);
}

void LuxxleWalletHandler::GetTransactionSimulationOptInStatusOptions(
    const base::Value::List& args) {
  base::Value::List list;
  list.Append(
      MakeSelectValue(l10n_util::GetStringUTF16(IDS_SETTINGS_SELECT_VALUE_ASK),
                      ::luxxle_wallet::mojom::BlowfishOptInStatus::kUnset));
  list.Append(
      MakeSelectValue(l10n_util::GetStringUTF16(IDS_SETTINGS_SELECT_VALUE_YES),
                      ::luxxle_wallet::mojom::BlowfishOptInStatus::kAllowed));
  list.Append(
      MakeSelectValue(l10n_util::GetStringUTF16(IDS_SETTINGS_SELECT_VALUE_NO),
                      ::luxxle_wallet::mojom::BlowfishOptInStatus::kDenied));

  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0], list);
}

void LuxxleWalletHandler::RemoveChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  AllowJavascript();

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  GetNetworkManager()->RemoveCustomNetwork(*chain_id, *coin);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void LuxxleWalletHandler::ResetChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  AllowJavascript();

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  DCHECK(GetNetworkManager()->CustomChainExists(*chain_id, *coin));
  GetNetworkManager()->RemoveCustomNetwork(*chain_id, *coin);
  DCHECK(GetNetworkManager()->KnownChainExists(*chain_id, *coin));
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void LuxxleWalletHandler::GetNetworksList(const base::Value::List& args) {
  CHECK_EQ(args.size(), 2U);
  base::Value::Dict result;
  auto coin = ToCoinType(args[1].GetIfInt());
  if (!coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  result.Set("defaultNetwork",
             GetNetworkManager()->GetCurrentChainId(*coin, std::nullopt));

  auto& networks = result.Set("networks", base::Value::List())->GetList();
  for (const auto& it : GetNetworkManager()->GetAllChains()) {
    if (it->coin == coin) {
      networks.Append(luxxle_wallet::NetworkInfoToValue(*it));
    }
  }
  auto& known_networks =
      result.Set("knownNetworks", base::Value::List())->GetList();
  for (const auto& it : GetNetworkManager()->GetAllKnownChains(*coin)) {
    known_networks.Append(it->chain_id);
  }

  auto& custom_networks =
      result.Set("customNetworks", base::Value::List())->GetList();
  for (const auto& it : GetNetworkManager()->GetAllCustomChains(*coin)) {
    custom_networks.Append(it->chain_id);
  }

  auto& hidden_networks =
      result.Set("hiddenNetworks", base::Value::List())->GetList();
  for (const auto& it : GetNetworkManager()->GetHiddenNetworks(*coin)) {
    hidden_networks.Append(it);
  }

  AllowJavascript();
  ResolveJavascriptCallback(args[0], result);
}

void LuxxleWalletHandler::GetPrepopulatedNetworksList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();

  base::Value::List networks;

  auto* blockchain_registry = luxxle_wallet::BlockchainRegistry::GetInstance();
  if (!blockchain_registry) {
    ResolveJavascriptCallback(args[0], networks);
    return;
  }

  for (const auto& it : blockchain_registry->GetPrepopulatedNetworks()) {
    networks.Append(luxxle_wallet::NetworkInfoToValue(*it));
  }

  ResolveJavascriptCallback(args[0], networks);
}

void LuxxleWalletHandler::OnAddChain(base::Value javascript_callback,
                                    const std::string& chain_id,
                                    luxxle_wallet::mojom::ProviderError error,
                                    const std::string& error_message) {
  base::Value::List result;
  result.Append(error == luxxle_wallet::mojom::ProviderError::kSuccess);
  result.Append(error_message);
  ResolveJavascriptCallback(javascript_callback, result);
  if (chain_callback_for_testing_) {
    std::move(chain_callback_for_testing_).Run();
  }
}

void LuxxleWalletHandler::AddChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 2U);
  AllowJavascript();
  auto* luxxle_wallet_service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
          Profile::FromWebUI(web_ui()));

  luxxle_wallet::mojom::NetworkInfoPtr chain =
      luxxle_wallet::ValueToNetworkInfo(args[1]);

  if (!chain || !luxxle_wallet_service) {
    base::Value::List result;
    result.Append(false);
    result.Append(l10n_util::GetStringUTF8(
        IDS_SETTINGS_WALLET_NETWORKS_SUMBISSION_FAILED));
    ResolveJavascriptCallback(args[0], result);
    return;
  }

  luxxle_wallet_service->json_rpc_service()->AddChain(
      std::move(chain),
      base::BindOnce(&LuxxleWalletHandler::OnAddChain,
                     weak_ptr_factory_.GetWeakPtr(), args[0].Clone()));
}

void LuxxleWalletHandler::SetDefaultNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  AllowJavascript();
  auto* luxxle_wallet_service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
          Profile::FromWebUI(web_ui()));
  auto result = luxxle_wallet_service
                    ? luxxle_wallet_service->json_rpc_service()->SetNetwork(
                          *chain_id, *coin, std::nullopt)
                    : false;
  ResolveJavascriptCallback(args[0], base::Value(result));
}

void LuxxleWalletHandler::AddHiddenNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  AllowJavascript();
  GetNetworkManager()->AddHiddenNetwork(*coin, *chain_id);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void LuxxleWalletHandler::RemoveHiddenNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  AllowJavascript();
  GetNetworkManager()->RemoveHiddenNetwork(*coin, *chain_id);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

PrefService* LuxxleWalletHandler::GetPrefs() {
  return Profile::FromWebUI(web_ui())->GetPrefs();
}

luxxle_wallet::NetworkManager* LuxxleWalletHandler::GetNetworkManager() {
  return luxxle_wallet::LuxxleWalletServiceFactory::GetInstance()
      ->GetServiceForContext(Profile::FromWebUI(web_ui()))
      ->network_manager();
}

void LuxxleWalletHandler::IsBitcoinEnabled(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(::luxxle_wallet::IsBitcoinEnabled()));
}

void LuxxleWalletHandler::IsZCashEnabled(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(::luxxle_wallet::IsZCashEnabled()));
}

void LuxxleWalletHandler::IsZCashShieldedTxEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0],
      base::Value(::luxxle_wallet::IsZCashShieldedTransactionsEnabled()));
}

void LuxxleWalletHandler::IsCardanoEnabled(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(::luxxle_wallet::IsCardanoEnabled()));
}

void LuxxleWalletHandler::IsTransactionSimulationsEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], base::Value(::luxxle_wallet::IsTransactionSimulationsEnabled()));
}

void LuxxleWalletHandler::SetWalletInPrivateWindowsEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 2U);
  bool enabled = args[1].GetBool();
  Profile::FromWebUI(web_ui())->GetPrefs()->SetBoolean(
      kLuxxleWalletPrivateWindowsEnabled, enabled);
  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void LuxxleWalletHandler::GetWalletInPrivateWindowsEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  bool enabled = Profile::FromWebUI(web_ui())->GetPrefs()->GetBoolean(
      kLuxxleWalletPrivateWindowsEnabled);
  AllowJavascript();
  ResolveJavascriptCallback(args[0], enabled);
}

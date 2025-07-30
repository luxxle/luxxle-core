/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_WALLET_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_WALLET_HANDLER_H_

#include <string>
#include <utility>

#include "base/memory/weak_ptr.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"

class PrefService;
class Profile;
class TestLuxxleWalletHandler;

class LuxxleWalletHandler : public settings::SettingsPageUIHandler {
 public:
  LuxxleWalletHandler();
  ~LuxxleWalletHandler() override;
  LuxxleWalletHandler(const LuxxleWalletHandler&) = delete;
  LuxxleWalletHandler& operator=(const LuxxleWalletHandler&) = delete;

  void SetChainCallbackForTesting(base::OnceClosure callback) {
    chain_callback_for_testing_ = std::move(callback);
  }

 private:
  friend TestLuxxleWalletHandler;
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void GetAutoLockMinutes(const base::Value::List& args);
  void GetSolanaProviderOptions(const base::Value::List& args);
  void GetTransactionSimulationOptInStatusOptions(
      const base::Value::List& args);
  void RemoveChain(const base::Value::List& args);
  void ResetChain(const base::Value::List& args);
  void GetNetworksList(const base::Value::List& args);
  void GetPrepopulatedNetworksList(const base::Value::List& args);
  void AddChain(const base::Value::List& args);
  void SetDefaultNetwork(const base::Value::List& args);
  void AddHiddenNetwork(const base::Value::List& args);
  void RemoveHiddenNetwork(const base::Value::List& args);
  void IsBitcoinEnabled(const base::Value::List& args);
  void IsZCashEnabled(const base::Value::List& args);
  void IsZCashShieldedTxEnabled(const base::Value::List& args);
  void IsCardanoEnabled(const base::Value::List& args);
  void IsTransactionSimulationsEnabled(const base::Value::List& args);
  void SetWalletInPrivateWindowsEnabled(const base::Value::List& args);
  void GetWalletInPrivateWindowsEnabled(const base::Value::List& args);

  PrefService* GetPrefs();
  luxxle_wallet::NetworkManager* GetNetworkManager();

  void OnAddChain(base::Value javascript_callback,
                  const std::string& chain_id,
                  luxxle_wallet::mojom::ProviderError error,
                  const std::string& error_message);

  base::OnceClosure chain_callback_for_testing_;
  base::WeakPtrFactory<LuxxleWalletHandler> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_WALLET_HANDLER_H_

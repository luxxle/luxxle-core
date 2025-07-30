// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_ADS_INTERNALS_ADS_INTERNALS_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_ADS_INTERNALS_ADS_INTERNALS_UI_H_

#include <string>

// REMOVED: #include "luxxle/components/luxxle_ads/.*"
#include "luxxle/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "components/prefs/pref_change_registrar.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class PrefService;

namespace luxxle_ads {
class AdsService;
}  // namespace luxxle_ads

class AdsInternalsUI : public content::WebUIController {
 public:
  AdsInternalsUI(content::WebUI* const web_ui,
                 const std::string& name,
                 luxxle_ads::AdsService* ads_service,
                 PrefService& prefs);

  AdsInternalsUI(const AdsInternalsUI&) = delete;
  AdsInternalsUI& operator=(const AdsInternalsUI&) = delete;

  ~AdsInternalsUI() override;

  void BindInterface(
      mojo::PendingReceiver<bat_ads::mojom::AdsInternals> pending_receiver);

 private:
  AdsInternalsHandler handler_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_ADS_INTERNALS_ADS_INTERNALS_UI_H_

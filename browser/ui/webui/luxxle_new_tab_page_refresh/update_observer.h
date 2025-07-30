// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_UPDATE_OBSERVER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_UPDATE_OBSERVER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "components/prefs/pref_change_registrar.h"

class PrefService;

namespace luxxle_new_tab_page_refresh {

class TopSitesFacade;

// Listens for changes to profile and system state that must be reflected on the
// new tab page.
class UpdateObserver {
 public:
  enum class Source {
    kBackgrounds,
    kSearch,
    kTopSites,
    kClock,
    kShieldsStats,
    kTalk,
    kRewards,
    kVPN
  };

  UpdateObserver(PrefService& pref_service, TopSitesFacade* top_sites_facade);
  ~UpdateObserver();

  UpdateObserver(const UpdateObserver&) = delete;
  UpdateObserver& operator=(const UpdateObserver&) = delete;

  void SetCallback(base::RepeatingCallback<void(Source)> callback);

 private:
  void OnUpdate(Source update_source);
  void OnPrefChanged(Source update_source, const std::string& path);
  void AddPrefListener(const std::string& path, Source update_source);

  PrefChangeRegistrar pref_change_registrar_;
  base::RepeatingCallback<void(Source)> callback_;
  base::WeakPtrFactory<UpdateObserver> weak_factory_{this};
};

}  // namespace luxxle_new_tab_page_refresh

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_UPDATE_OBSERVER_H_

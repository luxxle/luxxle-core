/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_BROWSER_COMMAND_CONTROLLER_H_
#define LUXXLE_BROWSER_UI_LUXXLE_BROWSER_COMMAND_CONTROLLER_H_

#include <optional>
#include <string>

#include "base/memory/raw_ref.h"
#include "base/scoped_observation.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data_observer.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "components/prefs/pref_change_registrar.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

class LuxxleAppMenuBrowserTest;
class LuxxleAppMenuModelBrowserTest;
class LuxxleBrowserCommandControllerTest;
enum class TabChangeType;

namespace content {
class WebContents;
}

// This namespace is needed for a chromium_src override
namespace chrome {

class LuxxleBrowserCommandController : public chrome::BrowserCommandController,
                                      public SplitViewBrowserDataObserver
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
    ,
                                      public luxxle_vpn::LuxxleVPNServiceObserver
#endif
{
 public:
  explicit LuxxleBrowserCommandController(Browser* browser);
  LuxxleBrowserCommandController(const LuxxleBrowserCommandController&) = delete;
  LuxxleBrowserCommandController& operator=(
      const LuxxleBrowserCommandController&) = delete;
  ~LuxxleBrowserCommandController() override;

#if BUILDFLAG(ENABLE_TOR)
  void UpdateCommandForTor();
#endif

 protected:
  void TabChangedAt(content::WebContents* contents,
                    int index,
                    TabChangeType change) override;
  void TabPinnedStateChanged(TabStripModel* tab_strip_model,
                             content::WebContents* contents,
                             int index) override;
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;
  void OnTabGroupChanged(const TabGroupChange& change) override;

 private:
  friend class ::LuxxleAppMenuBrowserTest;
  friend class ::LuxxleAppMenuModelBrowserTest;
  friend class ::LuxxleBrowserCommandControllerTest;

  // Overriden from SplitViewBrowserDataObserver:
  void OnTileTabs(const TabTile& tile) override;
  void OnWillBreakTile(const TabTile& tile) override;
  void OnWillDeleteBrowserData() override;

  // Overriden from CommandUpdater:
  bool SupportsCommand(int id) const override;
  bool IsCommandEnabled(int id) const override;
  bool ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition,
      base::TimeTicks time_stamp = base::TimeTicks::Now()) override;
  void AddCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(CommandObserver* observer) override;
  bool UpdateCommandEnabled(int id, bool state) override;

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  // luxxle_vpn::LuxxleVPNServiceObserver overrides:
  void OnPurchasedStateChanged(
      luxxle_vpn::mojom::PurchasedState state,
      const std::optional<std::string>& description) override;
#endif

  void InitLuxxleCommandState();
  void UpdateCommandForLuxxleRewards();
  void UpdateCommandForWebcompatReporter();
  void UpdateCommandForLuxxleSync();
  void UpdateCommandForLuxxleWallet();
  void UpdateCommandForSidebar();
  void UpdateCommandForAIChat();
  void UpdateCommandForLuxxleVPN();
  void UpdateCommandForPlaylist();
  void UpdateCommandForWaybackMachine();
  void UpdateCommandsForTabs();
  void UpdateCommandsForSend();
  void UpdateCommandsForPin();
  void UpdateCommandForSplitView();

  bool ExecuteLuxxleCommandWithDisposition(int id,
                                          WindowOpenDisposition disposition,
                                          base::TimeTicks time_stamp);

  PrefChangeRegistrar pref_change_registrar_;
  const raw_ref<Browser> browser_;

  CommandUpdaterImpl luxxle_command_updater_;

  base::ScopedObservation<SplitViewBrowserData, SplitViewBrowserDataObserver>
      split_view_browser_data_observation_{this};
};

}  // namespace chrome

#endif  // LUXXLE_BROWSER_UI_LUXXLE_BROWSER_COMMAND_CONTROLLER_H_

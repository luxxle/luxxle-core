/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/tabs/luxxle_tab_context_menu_contents.h"

#include <algorithm>
#include <iterator>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/notimplemented.h"
#include "base/notreached.h"
#include "luxxle/browser/ui/browser_commands.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_menu_model.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_strip_model.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data.h"
#include "luxxle/browser/ui/views/tabs/luxxle_browser_tab_strip_controller.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/tabs/tab_enums.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "components/sessions/core/tab_restore_service.h"
#include "content/public/browser/web_contents.h"
#include "ui/views/controls/menu/menu_runner.h"

LuxxleTabContextMenuContents::LuxxleTabContextMenuContents(
    Tab* tab,
    LuxxleBrowserTabStripController* controller,
    int index)
    : tab_(tab),
      tab_index_(index),
      browser_(const_cast<Browser*>(controller->browser())),
      controller_(controller) {
  const bool is_vertical_tab = tabs::utils::ShouldShowVerticalTabs(browser_);

  model_ = std::make_unique<LuxxleTabMenuModel>(
      this, controller->browser()->tab_menu_model_delegate(),
      controller->model(), index, is_vertical_tab);
  restore_service_ =
      TabRestoreServiceFactory::GetForProfile(browser_->profile());
  menu_runner_ = std::make_unique<views::MenuRunner>(
      model_.get(),
      views::MenuRunner::HAS_MNEMONICS | views::MenuRunner::CONTEXT_MENU,
      base::BindRepeating(&LuxxleTabContextMenuContents::OnMenuClosed,
                          weak_ptr_.GetWeakPtr()));
}

LuxxleTabContextMenuContents::~LuxxleTabContextMenuContents() = default;

void LuxxleTabContextMenuContents::Cancel() {
  controller_ = nullptr;
}

void LuxxleTabContextMenuContents::RunMenuAt(
    const gfx::Point& point,
    ui::mojom::MenuSourceType source_type) {
  menu_runner_->RunMenuAt(tab_->GetWidget(), nullptr,
                          gfx::Rect(point, gfx::Size()),
                          views::MenuAnchorPosition::kTopLeft, source_type);
}

bool LuxxleTabContextMenuContents::IsCommandIdChecked(int command_id) const {
  if (!IsValidContextMenu()) {
    return false;
  }

  if (command_id == LuxxleTabMenuModel::CommandShowVerticalTabs) {
    return tabs::utils::ShouldShowVerticalTabs(browser_);
  }

  return ui::SimpleMenuModel::Delegate::IsCommandIdChecked(command_id);
}

bool LuxxleTabContextMenuContents::IsCommandIdEnabled(int command_id) const {
  // This could be called after tab is closed.
  if (!IsValidContextMenu()) {
    return false;
  }

  if (IsLuxxleCommandId(command_id)) {
    return IsLuxxleCommandIdEnabled(command_id);
  }

  return controller_->IsCommandEnabledForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id), tab_);
}

bool LuxxleTabContextMenuContents::IsCommandIdVisible(int command_id) const {
  if (!IsValidContextMenu()) {
    return false;
  }

  if (command_id == LuxxleTabMenuModel::CommandShowVerticalTabs) {
    return tabs::utils::SupportsVerticalTabs(browser_);
  }

  if (command_id == LuxxleTabMenuModel::CommandBringAllTabsToThisWindow) {
    return luxxle::CanBringAllTabs(browser_);
  }

  return ui::SimpleMenuModel::Delegate::IsCommandIdVisible(command_id);
}

bool LuxxleTabContextMenuContents::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) const {
  if (!IsValidContextMenu()) {
    return false;
  }

  if (IsLuxxleCommandId(command_id)) {
    return false;
  }

  int browser_cmd;
  views::Widget* widget =
      BrowserView::GetBrowserViewForBrowser(browser_)->GetWidget();
  return TabStripModel::ContextMenuCommandToBrowserCommand(command_id,
                                                           &browser_cmd) &&
         widget->GetAccelerator(browser_cmd, accelerator);
}

void LuxxleTabContextMenuContents::ExecuteCommand(int command_id,
                                                 int event_flags) {
  if (!IsValidContextMenu()) {
    return;
  }

  if (IsLuxxleCommandId(command_id)) {
    return ExecuteLuxxleCommand(command_id);
  }

  // Executing the command destroys |this|, and can also end up destroying
  // |controller_|. So stop the highlights before executing the command.
  controller_->ExecuteCommandForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id), tab_);
}

bool LuxxleTabContextMenuContents::IsLuxxleCommandIdEnabled(
    int command_id) const {
  CHECK(IsValidContextMenu());

  switch (command_id) {
    case LuxxleTabMenuModel::CommandRestoreTab:
      return restore_service_ && (!restore_service_->IsLoaded() ||
                                  !restore_service_->entries().empty());
    case LuxxleTabMenuModel::CommandBookmarkAllTabs:
      if (browser_) {
        return browser_defaults::bookmarks_enabled &&
               chrome::CanBookmarkAllTabs(browser_);
      }
      break;
    case LuxxleTabMenuModel::CommandToggleTabMuted: {
      auto* model = static_cast<LuxxleTabStripModel*>(controller_->model());
      for (const auto& index : model->GetTabIndicesForCommandAt(tab_index_)) {
        if (!model->GetWebContentsAt(index)->GetLastCommittedURL().is_empty()) {
          return true;
        }
      }
      return false;
    }
    case LuxxleTabMenuModel::CommandCloseDuplicateTabs:
      return luxxle::HasDuplicateTabs(browser_);
    case LuxxleTabMenuModel::CommandShowVerticalTabs:
      [[fallthrough]];
    case LuxxleTabMenuModel::CommandBringAllTabsToThisWindow:
      [[fallthrough]];
    case LuxxleTabMenuModel::CommandNewSplitView:
      [[fallthrough]];
    case LuxxleTabMenuModel::CommandTileTabs:
      [[fallthrough]];
    case LuxxleTabMenuModel::CommandBreakTile:
      [[fallthrough]];
    case LuxxleTabMenuModel::CommandSwapTabsInTile:
      return true;
  }
  NOTREACHED() << "All commands are handled above";
}

void LuxxleTabContextMenuContents::ExecuteLuxxleCommand(int command_id) {
  CHECK(IsValidContextMenu());

  switch (command_id) {
    case LuxxleTabMenuModel::CommandRestoreTab:
      chrome::RestoreTab(browser_);
      return;
    case LuxxleTabMenuModel::CommandBookmarkAllTabs:
      chrome::BookmarkAllTabs(browser_);
      return;
    case LuxxleTabMenuModel::CommandShowVerticalTabs: {
      luxxle::ToggleVerticalTabStrip(browser_);
      BrowserView::GetBrowserViewForBrowser(browser_)->InvalidateLayout();
      return;
    }
    case LuxxleTabMenuModel::CommandToggleTabMuted: {
      auto* model = static_cast<LuxxleTabStripModel*>(controller_->model());
      auto indices = model->GetTabIndicesForCommandAt(tab_index_);
      std::vector<content::WebContents*> contentses;
      std::transform(
          indices.begin(), indices.end(), std::back_inserter(contentses),
          [&model](int index) { return model->GetWebContentsAt(index); });

      auto all_muted = model_->all_muted();
      for (auto* contents : contentses) {
        SetTabAudioMuted(contents, !all_muted, TabMutedReason::AUDIO_INDICATOR,
                         /*extension_id=*/std::string());
      }
      return;
    }
    case LuxxleTabMenuModel::CommandBringAllTabsToThisWindow: {
      luxxle::BringAllTabs(browser_);
      return;
    }
    case LuxxleTabMenuModel::CommandCloseDuplicateTabs:
      luxxle::CloseDuplicateTabs(browser_);
      return;
    case LuxxleTabMenuModel::CommandNewSplitView:
      NewSplitView();
      return;
    case LuxxleTabMenuModel::CommandTileTabs:
      TileSelectedTabs();
      return;
    case LuxxleTabMenuModel::CommandBreakTile:
      BreakSelectedTile();
      return;
    case LuxxleTabMenuModel::CommandSwapTabsInTile:
      SwapTabsInTile();
      return;
  }
  NOTREACHED() << "All commands are handled above";
}

bool LuxxleTabContextMenuContents::IsLuxxleCommandId(int command_id) const {
  return command_id > LuxxleTabMenuModel::CommandStart &&
         command_id < LuxxleTabMenuModel::CommandLast;
}

bool LuxxleTabContextMenuContents::IsValidContextMenu() const {
  if (menu_closed_) {
    return false;
  }

  return controller_->GetModelIndexOf(tab_).has_value() &&
         controller_->model()->ContainsIndex(tab_index_);
}

void LuxxleTabContextMenuContents::OnMenuClosed() {
  menu_closed_ = true;
}

void LuxxleTabContextMenuContents::NewSplitView() {
  auto* model = browser_->tab_strip_model();
  auto* tab = model->GetTabAtIndex(tab_index_);
  luxxle::NewSplitViewForTab(browser_, tab->GetHandle());
}

void LuxxleTabContextMenuContents::TileSelectedTabs() {
  luxxle::TileTabs(browser_, GetTabIndicesForSplitViewCommand());
}

void LuxxleTabContextMenuContents::BreakSelectedTile() {
  luxxle::BreakTiles(browser_, GetTabIndicesForSplitViewCommand());
}

void LuxxleTabContextMenuContents::SwapTabsInTile() {
  luxxle::SwapTabsInTile(browser_);
}

std::vector<int> LuxxleTabContextMenuContents::GetTabIndicesForSplitViewCommand()
    const {
  auto* model = static_cast<LuxxleTabStripModel*>(controller_->model());
  auto selected_indices = model->GetTabIndicesForCommandAt(tab_index_);
  if (base::Contains(selected_indices, tab_index_)) {
    return selected_indices;
  }

  return {tab_index_};
}

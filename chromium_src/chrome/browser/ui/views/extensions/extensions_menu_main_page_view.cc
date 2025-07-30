/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/views/extensions/extensions_menu_main_page_view.h"

#include "luxxle/browser/ui/views/extensions/luxxle_extension_menu_item_view.h"

#define LUXXLE_EXTENSION_MENU_MAIN_PAGE_VIEW_CREATE_AND_INSERT_MENU_ITEM        \
  {                                                                            \
    auto item = std::make_unique<LuxxleExtensionMenuItemView>(                  \
        browser_, is_enterprise, std::move(action_controller),                 \
        base::BindRepeating(&ExtensionsMenuHandler::OnExtensionToggleSelected, \
                            base::Unretained(menu_handler_), extension_id),    \
        base::BindRepeating(&ExtensionsMenuHandler::OpenSitePermissionsPage,   \
                            base::Unretained(menu_handler_), extension_id));   \
    item->Update(site_access_toggle_state, site_permissions_button_state,      \
                 site_permissions_button_access, is_enterprise);               \
    menu_items_->AddChildViewAt(std::move(item), index);                       \
    return;                                                                    \
  }

#include "src/chrome/browser/ui/views/extensions/extensions_menu_main_page_view.cc"

#undef LUXXLE_EXTENSION_MENU_MAIN_PAGE_VIEW_CREATE_AND_INSERT_MENU_ITEM

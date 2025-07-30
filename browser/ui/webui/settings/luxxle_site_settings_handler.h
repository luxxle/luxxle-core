/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_SITE_SETTINGS_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_SITE_SETTINGS_HANDLER_H_

#include <string>
#include <vector>

#include "chrome/browser/ui/webui/settings/site_settings_handler.h"

namespace settings {

class LuxxleSiteSettingsHandler : public SiteSettingsHandler {
 public:
  explicit LuxxleSiteSettingsHandler(Profile* profile);

  LuxxleSiteSettingsHandler(const LuxxleSiteSettingsHandler&) = delete;
  LuxxleSiteSettingsHandler& operator=(const LuxxleSiteSettingsHandler&) = delete;

  ~LuxxleSiteSettingsHandler() override;

  // SettingsPageUIHandler:
  void RegisterMessages() override;

  // Returns whether the pattern is valid given the type.
  void HandleIsPatternValidForType(const base::Value::List& args);

  bool IsPatternValidForLuxxleContentType(ContentSettingsType content_type,
                                         const std::string& pattern_string);

  void RemoveNonModelData(const std::vector<url::Origin>& origins) override;

 private:
  friend class TestLuxxleSiteSettingsHandlerUnittest;
};

}  // namespace settings

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_SITE_SETTINGS_HANDLER_H_

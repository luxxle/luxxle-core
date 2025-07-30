/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_theme_event_router.h"

#include <memory>
#include <string>
#include <utility>

#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/common/extensions/api/luxxle_theme.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"
#include "ui/native_theme/native_theme.h"

namespace extensions {

LuxxleThemeEventRouter::LuxxleThemeEventRouter(Profile* profile)
    : profile_(profile) {
  observer_.Observe(ui::NativeTheme::GetInstanceForNativeUi());
}

LuxxleThemeEventRouter::~LuxxleThemeEventRouter() = default;

void LuxxleThemeEventRouter::OnNativeThemeUpdated(
    ui::NativeTheme* observed_theme) {
  DCHECK(observer_.IsObservingSource(observed_theme));
  Notify();
}

void LuxxleThemeEventRouter::Notify() {
  const std::string theme_type =
      dark_mode::GetStringFromLuxxleDarkModeType(
          dark_mode::GetActiveLuxxleDarkModeType());

  auto event = std::make_unique<extensions::Event>(
      extensions::events::LUXXLE_ON_LUXXLE_THEME_TYPE_CHANGED,
      api::luxxle_theme::OnLuxxleThemeTypeChanged::kEventName,
      api::luxxle_theme::OnLuxxleThemeTypeChanged::Create(theme_type),
      profile_);

  if (EventRouter* event_router = EventRouter::Get(profile_))
    event_router->BroadcastEvent(std::move(event));
}

}  // namespace extensions

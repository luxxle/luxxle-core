/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_EXTENSIONS_LUXXLE_THEME_EVENT_ROUTER_H_
#define LUXXLE_BROWSER_EXTENSIONS_LUXXLE_THEME_EVENT_ROUTER_H_

#include "base/memory/raw_ptr.h"
#include "base/scoped_observation.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/native_theme_observer.h"

class Profile;

namespace extensions {

class LuxxleThemeEventRouter : public ui::NativeThemeObserver {
 public:
  explicit LuxxleThemeEventRouter(Profile* profile);
  LuxxleThemeEventRouter(const LuxxleThemeEventRouter&) = delete;
  LuxxleThemeEventRouter& operator=(const LuxxleThemeEventRouter&) = delete;
  ~LuxxleThemeEventRouter() override;

 private:
  friend class MockLuxxleThemeEventRouter;

  // ui::NativeThemeObserver overrides:
  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;

  // Make virtual for testing.
  virtual void Notify();

  raw_ptr<ui::NativeTheme> current_native_theme_for_testing_ = nullptr;
  raw_ptr<Profile> profile_ = nullptr;
  base::ScopedObservation<ui::NativeTheme, ui::NativeThemeObserver> observer_{
      this};
};

}  // namespace extensions

#endif  // LUXXLE_BROWSER_EXTENSIONS_LUXXLE_THEME_EVENT_ROUTER_H_

/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_
#define LUXXLE_BROWSER_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/geolocation/geolocation_permission_context_delegate.h"

class Profile;

class LuxxleGeolocationPermissionContextDelegate
    : public GeolocationPermissionContextDelegate {
 public:
  explicit LuxxleGeolocationPermissionContextDelegate(
      content::BrowserContext* browser_context);
  LuxxleGeolocationPermissionContextDelegate(
      const LuxxleGeolocationPermissionContextDelegate&) = delete;
  LuxxleGeolocationPermissionContextDelegate& operator=(
      const LuxxleGeolocationPermissionContextDelegate&) = delete;
  ~LuxxleGeolocationPermissionContextDelegate() override;

  bool DecidePermission(
      const permissions::PermissionRequestID& id,
      const GURL& requesting_origin,
      bool user_gesture,
      permissions::BrowserPermissionCallback* callback,
      permissions::GeolocationPermissionContext* context) override;

 private:
  raw_ptr<Profile> profile_ = nullptr;
};

#endif  // LUXXLE_BROWSER_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_CONTEXT_DELEGATE_H_

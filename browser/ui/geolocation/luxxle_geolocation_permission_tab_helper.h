/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_TAB_HELPER_H_
#define LUXXLE_BROWSER_UI_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_TAB_HELPER_H_

#include "luxxle/components/luxxle_geolocation_permission/common/luxxle_geolocation_permission.mojom.h"
#include "content/public/browser/render_frame_host_receiver_set.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class LuxxleGeolocationPermissionTabHelper final
    : public content::WebContentsObserver,
      public geolocation::mojom::LuxxleGeolocationPermission,
      public content::WebContentsUserData<LuxxleGeolocationPermissionTabHelper> {
 public:
  explicit LuxxleGeolocationPermissionTabHelper(content::WebContents* contents);
  ~LuxxleGeolocationPermissionTabHelper() override;

  static void BindLuxxleGeolocationPermission(
      mojo::PendingAssociatedReceiver<
          geolocation::mojom::LuxxleGeolocationPermission> receiver,
      content::RenderFrameHost* rfh);

  // content::WebContentsObserver
  void PrimaryPageChanged(content::Page& page) override;

  // geolocation::mojom::LuxxleGeolocationPermission overrides:
  void SetEnableHighAccuracy(bool high_accuracy) override;

  bool enable_high_accuracy() const { return enable_high_accuracy_; }

  WEB_CONTENTS_USER_DATA_KEY_DECL();

 private:
  content::RenderFrameHostReceiverSet<
      geolocation::mojom::LuxxleGeolocationPermission>
      luxxle_geolocation_permission_receivers_;

  bool enable_high_accuracy_ = false;
};

#endif  // LUXXLE_BROWSER_UI_GEOLOCATION_LUXXLE_GEOLOCATION_PERMISSION_TAB_HELPER_H_

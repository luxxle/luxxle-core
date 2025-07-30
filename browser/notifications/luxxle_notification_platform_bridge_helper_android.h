/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NOTIFICATIONS_LUXXLE_NOTIFICATION_PLATFORM_BRIDGE_HELPER_ANDROID_H_
#define LUXXLE_BROWSER_NOTIFICATIONS_LUXXLE_NOTIFICATION_PLATFORM_BRIDGE_HELPER_ANDROID_H_

#include <string>

#include "url/gurl.h"

class LuxxleNotificationPlatformBridgeHelperAndroid {
 public:
  static void MaybeRegenerateNotification(const std::string& notification_id,
                                          const GURL& service_worker_scope);
};

#endif  // LUXXLE_BROWSER_NOTIFICATIONS_LUXXLE_NOTIFICATION_PLATFORM_BRIDGE_HELPER_ANDROID_H_

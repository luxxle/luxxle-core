/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/notifications/ads_notification_handler.h"

#define LUXXLE_ADD_LUXXLE_ADS_NOTIFICATION_HANDLER \
  AddNotificationHandler(                        \
      NotificationHandler::Type::LUXXLE_ADS,      \
      std::make_unique<luxxle_ads::AdsNotificationHandler>(*profile));
#include "src/chrome/browser/notifications/notification_display_service_impl.cc"
#undef LUXXLE_ADD_LUXXLE_ADS_NOTIFICATION_HANDLER

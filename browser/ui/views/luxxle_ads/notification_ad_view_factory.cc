/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_ads/notification_ad_view_factory.h"

#include "luxxle/browser/ui/views/luxxle_ads/text_notification_ad_view.h"

namespace luxxle_ads {

// static
std::unique_ptr<NotificationAdView> NotificationAdViewFactory::Create(
    const NotificationAd& notification_ad) {
  return std::make_unique<TextNotificationAdView>(notification_ad);
}

}  // namespace luxxle_ads

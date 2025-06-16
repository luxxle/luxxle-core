/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_LUXXLE_APP_CONTROLLER_MAC_H_
#define LUXXLE_BROWSER_LUXXLE_APP_CONTROLLER_MAC_H_

#import <Cocoa/Cocoa.h>

#import "chrome/browser/app_controller_mac.h"

// * Manages logic to switch hotkey between copy and copy clean link item.
// * Add "New Private Window with Tor" to the dock menu.
@interface LuxxleAppController : AppController
@end

#endif  // LUXXLE_BROWSER_LUXXLE_APP_CONTROLLER_MAC_H_

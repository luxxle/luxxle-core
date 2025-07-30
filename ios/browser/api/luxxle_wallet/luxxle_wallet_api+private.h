/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_api.h"

NS_ASSUME_NONNULL_BEGIN

class ProfileIOS;

@interface LuxxleWalletAPI (Private)
- (instancetype)initWithBrowserState:(ProfileIOS*)profile;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_PRIVATE_H_

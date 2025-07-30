/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_P3A_LUXXLE_P3A_UTILS_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_P3A_LUXXLE_P3A_UTILS_PRIVATE_H_

#include "luxxle/ios/browser/api/p3a/luxxle_p3a_utils.h"

class PrefService;

namespace p3a {
class P3AService;
}  // namespace p3a

NS_ASSUME_NONNULL_BEGIN

@interface LuxxleP3AUtils (Private)
- (instancetype)initWithLocalState:(PrefService*)localState
                        p3aService:(scoped_refptr<p3a::P3AService>)p3aService;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_P3A_LUXXLE_P3A_UTILS_PRIVATE_H_

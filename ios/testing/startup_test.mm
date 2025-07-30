/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#import <XCTest/XCTest.h>
#import "luxxle/ios/app/luxxle_core_main.h"

@interface LuxxleCoreMainStartupTest : XCTestCase
@end

@implementation LuxxleCoreMainStartupTest

- (void)testLuxxleCoreMainStartup {
  auto core = [[LuxxleCoreMain alloc] initWithUserAgent:@"UnitTest"];
  [core scheduleLowPriorityStartupTasks];
}

@end

/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/opentabs/luxxle_tabgenerator_api.h"

#include <memory>

#include "base/memory/raw_ptr.h"
#include "base/strings/sys_string_conversions.h"
#import "luxxle/ios/browser/api/web/web_state/web_state+private.h"
#include "ios/chrome/browser/shared/model/browser/browser.h"
#include "ios/web/public/thread/web_thread.h"
#include "ios/web/web_state/web_state_impl.h"
#include "net/base/apple/url_conversions.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#pragma mark - LuxxleSyncTab

@interface LuxxleSyncTab ()
@property(nonatomic, strong) WebState* web_state;
@end

@implementation LuxxleSyncTab

- (instancetype)initWithBrowser:(Browser*)browser
                 isOffTheRecord:(bool)isOffTheRecord {
  if ((self = [super init])) {
    _web_state = [[WebState alloc] initWithBrowser:browser
                                    isOffTheRecord:isOffTheRecord];
  }
  return self;
}

- (WebState*)webState {
  return _web_state;
}

- (void)setTitle:(NSString*)title {
  [self.web_state setTitle:title];
}

- (void)setURL:(NSURL*)url {
  [self.web_state setURL:url];
}
@end

#pragma mark - LuxxleTabGeneratorAPI

@interface LuxxleTabGeneratorAPI () {
  raw_ptr<Browser> browser_;
  raw_ptr<Browser> otr_browser_;
}
@end

@implementation LuxxleTabGeneratorAPI

- (instancetype)initWithBrowser:(Browser*)browser
                     otrBrowser:(Browser*)otrBrowser {
  if ((self = [super init])) {
    DCHECK_CURRENTLY_ON(web::WebThread::UI);
    DCHECK(browser);
    DCHECK(otrBrowser);
    browser_ = browser;
    otr_browser_ = otrBrowser;
  }
  return self;
}

- (LuxxleSyncTab*)createLuxxleSyncTab:(bool)isOffTheRecord {
  return [[LuxxleSyncTab alloc]
      initWithBrowser:isOffTheRecord ? otr_browser_ : browser_
       isOffTheRecord:isOffTheRecord];
}

- (void)dealloc {
  browser_ = nil;
}

@end

// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_H_
#define LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_H_

#import <Foundation/Foundation.h>

@class LuxxleBookmarksAPI, LuxxleHistoryAPI, LuxxlePasswordAPI, LuxxleOpenTabsAPI,
    LuxxleSendTabAPI, LuxxleSyncAPI, LuxxleSyncProfileServiceIOS, DeAmpPrefs,
    LuxxleTabGeneratorAPI, LuxxleWalletAPI, LuxxleStats, AIChat,
    DefaultHostContentSettings, CWVWebViewConfiguration, WebImageDownloader,
    NTPBackgroundImagesService;
@protocol AIChatDelegate
, IpfsAPI;

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface LuxxleProfileController : NSObject
@property(readonly) LuxxleBookmarksAPI* bookmarksAPI;
@property(readonly) LuxxleHistoryAPI* historyAPI;
@property(readonly) LuxxlePasswordAPI* passwordAPI;
@property(readonly) LuxxleOpenTabsAPI* openTabsAPI;
@property(readonly) LuxxleSendTabAPI* sendTabAPI;
@property(readonly) LuxxleSyncAPI* syncAPI;
@property(readonly) LuxxleSyncProfileServiceIOS* syncProfileService;
@property(readonly) LuxxleTabGeneratorAPI* tabGeneratorAPI;
@property(readonly) LuxxleWalletAPI* luxxleWalletAPI;
@property(readonly) LuxxleStats* luxxleStats;
@property(readonly) DeAmpPrefs* deAmpPrefs;
@property(readonly) id<IpfsAPI> ipfsAPI;
- (AIChat*)aiChatAPIWithDelegate:(id<AIChatDelegate>)delegate;
/// The default content settings for regular browsing windows
@property(readonly) DefaultHostContentSettings* defaultHostContentSettings;
@property(readonly) NTPBackgroundImagesService* backgroundImagesService;
@property(readonly) WebImageDownloader* webImageDownloader;

@property(readonly) CWVWebViewConfiguration* defaultWebViewConfiguration;
@property(readonly) CWVWebViewConfiguration* nonPersistentWebViewConfiguration;
- (void)notifyLastPrivateTabClosed;

@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_H_

/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_PROVIDER_DELEGATE_IOS_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_PROVIDER_DELEGATE_IOS_H_

#import <Foundation/Foundation.h>
#import "luxxle_wallet.mojom.objc.h"

@class URLOriginIOS;

NS_ASSUME_NONNULL_BEGIN

typedef void (^RequestPermissionsCallback)(
    LuxxleWalletRequestPermissionsError error,
    NSArray<NSString*>* _Nullable allowedAccounts);

OBJC_EXPORT
@protocol LuxxleWalletProviderDelegate
- (bool)isTabVisible;
- (void)showPanel;
- (URLOriginIOS*)getOrigin;
- (void)walletInteractionDetected;
- (void)showWalletOnboarding;
- (void)showWalletBackup;
- (void)unlockWallet;
- (void)showAccountCreation:(LuxxleWalletCoinType)type;
- (void)requestPermissions:(LuxxleWalletCoinType)type
                  accounts:(NSArray<NSString*>*)accounts
                completion:(RequestPermissionsCallback)completion;
- (bool)isAccountAllowed:(LuxxleWalletCoinType)type account:(NSString*)account;
- (nullable NSArray<NSString*>*)getAllowedAccounts:(LuxxleWalletCoinType)type
                                          accounts:
                                              (NSArray<NSString*>*)accounts;
- (bool)isPermissionDenied:(LuxxleWalletCoinType)type;
- (void)addSolanaConnectedAccount:(NSString*)account;
- (void)removeSolanaConnectedAccount:(NSString*)account;
- (bool)isSolanaAccountConnected:(NSString*)account;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_PROVIDER_DELEGATE_IOS_H_

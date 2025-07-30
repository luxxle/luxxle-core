/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_H_

#import <Foundation/Foundation.h>

@protocol LuxxleWalletBlockchainRegistry;
@protocol LuxxleWalletEthereumProvider;
@protocol LuxxleWalletProviderDelegate;
@protocol LuxxleWalletSolanaProvider;
@protocol LuxxleWalletLuxxleWalletP3A;

typedef NS_ENUM(NSInteger, LuxxleWalletCoinType);

NS_ASSUME_NONNULL_BEGIN

typedef NSString* LuxxleWalletProviderScriptKey NS_STRING_ENUM;
OBJC_EXPORT LuxxleWalletProviderScriptKey const
    LuxxleWalletProviderScriptKeyEthereum;
OBJC_EXPORT LuxxleWalletProviderScriptKey const
    LuxxleWalletProviderScriptKeySolana;
OBJC_EXPORT LuxxleWalletProviderScriptKey const
    LuxxleWalletProviderScriptKeySolanaWeb3;
OBJC_EXPORT LuxxleWalletProviderScriptKey const
    LuxxleWalletProviderScriptKeyWalletStandard;

OBJC_EXPORT
@interface LuxxleWallet : NSObject
- (instancetype)init NS_UNAVAILABLE;
@end

OBJC_EXPORT
@interface LuxxleWalletAPI : NSObject

@property(class, readonly) id<LuxxleWalletBlockchainRegistry> blockchainRegistry;

- (nullable id<LuxxleWalletEthereumProvider>)
    ethereumProviderWithDelegate:(id<LuxxleWalletProviderDelegate>)delegate
               isPrivateBrowsing:(bool)isPrivateBrowsing NS_SWIFT_UI_ACTOR;

- (nullable id<LuxxleWalletSolanaProvider>)
    solanaProviderWithDelegate:(id<LuxxleWalletProviderDelegate>)delegate
             isPrivateBrowsing:(bool)isPrivateBrowsing NS_SWIFT_UI_ACTOR;

- (NSDictionary<LuxxleWalletProviderScriptKey, NSString*>*)
    providerScriptsForCoinType:(LuxxleWalletCoinType)coinType;

- (nullable id<LuxxleWalletLuxxleWalletP3A>)walletP3A;

- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_WALLET_LUXXLE_WALLET_API_H_

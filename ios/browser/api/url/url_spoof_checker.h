/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_
#define LUXXLE_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// The |SkeletonType| and |TopDomainEntry| are mirrored in trie_entry.h. These
/// are used to insert and read nodes from the Trie.
/// The type of skeleton in the trie node.
typedef NSInteger LuxxleSpoofCheckerSkeletonType NS_TYPED_ENUM
    NS_SWIFT_NAME(URLSpoofChecker.SkeletonType);
OBJC_EXPORT LuxxleSpoofCheckerSkeletonType const
    LuxxleSpoofCheckerSkeletonTypeFull;
OBJC_EXPORT LuxxleSpoofCheckerSkeletonType const
    LuxxleSpoofCheckerSkeletonTypeSeparatorsRemoved;

typedef NSInteger LuxxleSpoofCheckerLookalikeURLMatchType NS_TYPED_ENUM
    NS_SWIFT_NAME(URLSpoofChecker.LookalikeURLMatchType);

OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeNone;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeSkeletonMatchSiteEngagement;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeEditDistance;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeEditDistanceSiteEngagement;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeTargetEmbedding;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeSkeletonMatchTop500;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeSkeletonMatchTop5k;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeTargetEmbeddingForSafetyTips;
/// The domain name failed IDN spoof checks but didn't match a safe hostname.
/// As a result, there is no URL to suggest to the user in the form of "Did
/// you mean <url>?".
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeFailedSpoofChecks;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeCharacterSwapSiteEngagement;
OBJC_EXPORT LuxxleSpoofCheckerLookalikeURLMatchType const
    LuxxleSpoofCheckerLookalikeURLMatchTypeCharacterSwapTop500;

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker.TopDomainEntry)
@interface URLSpoofCheckerTopDomainEntry : NSObject
- (instancetype)init NS_UNAVAILABLE;
/// The domain name.
@property(nonatomic, readonly) NSString* domain;
/// True if the domain is in the top 1000 bucket.
@property(nonatomic, readonly) bool isTopBucket;
/// Type of the skeleton stored in the trie node.
@property(nonatomic, readonly) LuxxleSpoofCheckerSkeletonType skeletonType;
@end

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker.Result)
@interface LuxxleURLSpoofCheckerResult : NSObject
- (instancetype)init NS_UNAVAILABLE;
@property(nonatomic, readonly)
    LuxxleSpoofCheckerLookalikeURLMatchType urlMatchType;
@property(nonatomic, readonly, nullable) NSURL* suggestedURL;
@end

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker)
@interface LuxxleURLSpoofChecker : NSObject
- (instancetype)init NS_UNAVAILABLE;
+ (URLSpoofCheckerTopDomainEntry*)getSimilarTopDomain:(NSString*)hostname;
+ (URLSpoofCheckerTopDomainEntry*)lookupSkeletonInTopDomains:
    (NSString*)hostname;
+ (NSArray<NSString*>*)getSkeletons:(NSString*)url;
+ (LuxxleURLSpoofCheckerResult*)isLookalikeURL:(NSString*)url;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_

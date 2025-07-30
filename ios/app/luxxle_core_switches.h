/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_APP_LUXXLE_CORE_SWITCHES_H_
#define LUXXLE_IOS_APP_LUXXLE_CORE_SWITCHES_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSString* LuxxleCoreSwitchKey NS_STRING_ENUM;
/// Overrides the component updater source. Defaults to the CI provided value
///
/// Expected value: url-source={url}
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyComponentUpdater;
/// Overrides Chromium VLOG verbosity. Defaults to only printing from folders
/// existing within a `luxxle` subfolder up to level 0.
///
/// Expected value: {folder-expression}={level}
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVModule;
/// Overrides the sync service base URL. Defaults to the CI provided value
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeySyncURL;
/// Sets the variations URL to a test seed generated in luxxle/luxxle-variations
/// pull request.
///
/// Expected value: A luxxle/luxxle-variations pull request number
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVariationsPR;
/// Overrides the variations seed URL. Defaults to production
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVariationsURL;
/// Sets a number of overrides for the ads & ledger services such as which
/// environment its using, debug mode, etc.
///
/// Expected value: A comma-separated list of flags, including:
///     - staging={bool}
////    - development={bool}
///     - debug={bool}
///     - reconcile-interval={int}
///     - retry-interval={int}
OBJC_EXPORT const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyRewardsFlags;
/// Overrides the number of seconds to upload P3A metrics
///
/// Expected value: A number (in seconds)
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AUploadIntervalSeconds NS_SWIFT_NAME(p3aUploadIntervalSeconds);  // NOLINT
/// Avoid upload interval randomization
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3ADoNotRandomizeUploadInterval NS_SWIFT_NAME(p3aDoNotRandomizeUploadInterval);  // NOLINT
/// Interval between restarting the uploading process for all gathered values
///
/// Expected value: A number (in seconds)
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3ATypicalRotationIntervalSeconds NS_SWIFT_NAME(p3aTypicalRotationIntervalSeconds);  // NOLINT
/// Interval between restarting the uploading process for all gathered values
///
/// Expected value: A number (in seconds)
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AExpressRotationIntervalSeconds NS_SWIFT_NAME(p3aExpressRotationIntervalSeconds);  // NOLINT
/// Interval between restarting the uploading process for all gathered values
///
/// Expected value: A number (in seconds)
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3ASlowRotationIntervalSeconds NS_SWIFT_NAME(p3aSlowRotationIntervalSeconds);  // NOLINT
/// For specifying a fake STAR epoch for the typical cadence, for the purpose of
/// triggering the transmission of encrypted measurements before they are
/// due to be sent, for testing purposes.
///
/// Expected value: An 8-bit unsigned integer
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AFakeTypicalStarEpoch NS_SWIFT_NAME(p3aFakeTypicalStarEpoch);  // NOLINT
/// For specifying a fake STAR epoch for the slow cadence, for the purpose of
/// triggering the transmission of encrypted measurements before they are
/// due to be sent, for testing purposes.
///
/// Expected value: An 8-bit unsigned integer
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AFakeSlowStarEpoch NS_SWIFT_NAME(p3aFakeSlowStarEpoch);  // NOLINT
/// For specifying a fake STAR epoch for the express cadence, for the purpose of
/// triggering the transmission of encrypted measurements before they are
/// due to be sent, for testing purposes.
///
/// Expected value: An 8-bit unsigned integer
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AFakeExpressStarEpoch NS_SWIFT_NAME(p3aFakeExpressStarEpoch);  // NOLINT
/// Overrides the P3A JSON backend URL.
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AJsonUploadServerURL NS_SWIFT_NAME(p3aJsonUploadServerURL);
/// Overrides the P3A Creative JSON backend URL.
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3ACreativeUploadServerURL NS_SWIFT_NAME(p3aCreativeUploadServerURL);
/// Overrides the P2A JSON backend URL.
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP2AJsonUploadServerURL NS_SWIFT_NAME(p2aJsonUploadServerURL);
/// Overrides the P3A Constellation backend URL.
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AConstellationUploadServerHost NS_SWIFT_NAME(p3aConstellationUploadServerHost);
/// Overrides the P3A "disable star attestation" setting.
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3ADisableStarAttestation NS_SWIFT_NAME(p3aDisableStarAttestation);
/// Overrides the P3A STAR randomness host.
///
/// Expected value: A URL string
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AStarRandomnessHost NS_SWIFT_NAME(p3aStarRandomnessHost);
/// Do not try to resent values even if a cloud returned an HTTP error, just
/// continue the normal process.
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyP3AIgnoreServerErrors NS_SWIFT_NAME(p3aIgnoreServerErrors);  // NOLINT
/// Fetch CRX components from staging instead of production
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyUseDevGoUpdater NS_SWIFT_NAME(useDevGoUpdater);  // NOLINT
/// Expected value: A string {dev, staging, prod}
OBJC_EXPORT const LuxxleCoreSwitchKey
    LuxxleCoreSwitchKeyServicesEnvironment NS_SWIFT_NAME(servicesEnvironment);

/// Defines a switch that may be overriden on launch.
///
/// These are essentially the same as passing a command line argument in the
/// format `--key=value` (or `--key` if you do not require a value)
OBJC_EXPORT
@interface LuxxleCoreSwitch : NSObject
@property(readonly) LuxxleCoreSwitchKey key;
@property(readonly, nullable) NSString* value;
- (instancetype)initWithKey:(LuxxleCoreSwitchKey)key;
- (instancetype)initWithKey:(LuxxleCoreSwitchKey)key
                      value:(nullable NSString*)value NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_APP_LUXXLE_CORE_SWITCHES_H_

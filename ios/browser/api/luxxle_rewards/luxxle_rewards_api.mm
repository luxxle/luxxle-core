/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#import "luxxle/ios/browser/api/luxxle_rewards/luxxle_rewards_api.h"

#import <UIKit/UIKit.h>

#include <optional>

#include "base/containers/flat_map.h"
#include "base/ios/ios_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/time/time.h"
#include "base/types/cxx23_to_underlying.h"
#include "base/values.h"
#include "luxxle/build/ios/mojom/cpp_transformations.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#import "luxxle/ios/browser/api/luxxle_rewards/rewards.mojom.objc+private.h"
#import "luxxle/ios/browser/api/luxxle_rewards/rewards_client_bridge.h"
#import "luxxle/ios/browser/api/luxxle_rewards/rewards_client_ios.h"
#import "luxxle/ios/browser/api/luxxle_rewards/rewards_notification.h"
#import "luxxle/ios/browser/api/luxxle_rewards/rewards_observer.h"
#import "luxxle/ios/browser/api/common/common_operations.h"
#include "components/os_crypt/sync/os_crypt.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "url/gurl.h"
#include "url/origin.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {

using luxxle_rewards::RemoteWorker;
using luxxle_rewards::mojom::RewardsEngineClient;

class RewardsDatabaseWorker
    : public RemoteWorker<luxxle_rewards::mojom::RewardsDatabase> {
 public:
  RewardsDatabaseWorker() : RemoteWorker(CreateTaskRunner()) {}
  ~RewardsDatabaseWorker() = default;

 private:
  static scoped_refptr<base::SequencedTaskRunner> CreateTaskRunner() {
    return base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
  }
};

class HashPrefixStoreWorker
    : public RemoteWorker<luxxle_rewards::mojom::HashPrefixStore> {
 public:
  HashPrefixStoreWorker() : RemoteWorker(CreateTaskRunner()) {}
  ~HashPrefixStoreWorker() = default;

 private:
  static scoped_refptr<base::SequencedTaskRunner> CreateTaskRunner() {
    return base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN});
  }
};

class RewardsEngineWorker
    : public RemoteWorker<luxxle_rewards::mojom::RewardsEngine> {
 public:
  RewardsEngineWorker() : RemoteWorker(CreateTaskRunner()) {}
  ~RewardsEngineWorker() = default;

 private:
  static scoped_refptr<base::SequencedTaskRunner> CreateTaskRunner() {
    // The `WithBaseSyncPrimitives` flag is required due to the usage of [Sync]
    // interface method calls by the Rewards engine.
    return base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::WithBaseSyncPrimitives(),
         base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
  }
};

}  // namespace

#define LLOG(verbose_level, format, ...)                  \
  [self log:(__FILE__)                                    \
       line:(__LINE__)verboseLevel:(verbose_level)message \
           :base::SysNSStringToUTF8(                      \
                [NSString stringWithFormat:(format), ##__VA_ARGS__])]

NSString* const LuxxleRewardsErrorDomain = @"LuxxleRewardsErrorDomain";
NSNotificationName const LuxxleRewardsNotificationAdded =
    @"BATLuxxleRewardsNotificationAdded";

/// ---

@interface LuxxleRewardsAPI () <RewardsClientBridge> {
  scoped_refptr<base::SequencedTaskRunner> _taskRunner;
  std::unique_ptr<RewardsClientIOS> _rewardsClient;
  RewardsEngineWorker _rewardsEngine;
  RewardsDatabaseWorker _rewardsDatabase;
  HashPrefixStoreWorker _creatorPrefixStore;
}

@property(nonatomic, copy) NSString* storagePath;
@property(nonatomic) LuxxleRewardsRewardsParameters* rewardsParameters;
@property(nonatomic) LuxxleRewardsBalance* balance;
@property(nonatomic) dispatch_queue_t fileWriteThread;
@property(nonatomic) NSMutableDictionary<NSString*, NSString*>* state;
@property(nonatomic) LuxxleCommonOperations* commonOps;
@property(nonatomic) PrefService* profilePrefService;

@property(nonatomic) NSHashTable<RewardsObserver*>* observers;

@property(nonatomic, getter=isInitialized) BOOL initialized;
@property(nonatomic) BOOL initializing;
@property(nonatomic) BOOL dataMigrationFailed;
@property(nonatomic) LuxxleRewardsResult initializationResult;
@property(nonatomic, getter=isLoadingPublisherList) BOOL loadingPublisherList;
@property(nonatomic, getter=isInitializingWallet) BOOL initializingWallet;

/// Temporary blocks

@end

@implementation LuxxleRewardsAPI

- (instancetype)initWithStateStoragePath:(NSString*)path {
  if ((self = [super init])) {
    _taskRunner = base::SequencedTaskRunner::GetCurrentDefault();

    self.storagePath = path;
    self.commonOps =
        [[LuxxleCommonOperations alloc] initWithStoragePath:path
                                                taskRunner:_taskRunner];
    [self initProfilePrefService];
    [self maybeMigrateProfilePrefs];

    self.state = [[NSMutableDictionary alloc]
                     initWithContentsOfFile:self.randomStatePath]
                     ?: [[NSMutableDictionary alloc] init];
    self.fileWriteThread =
        dispatch_queue_create("com.rewards.file-write", DISPATCH_QUEUE_SERIAL);
    self.observers = [NSHashTable weakObjectsHashTable];

    _rewardsClient = std::make_unique<RewardsClientIOS>(self);

    _rewardsEngine.BindRemote<luxxle_rewards::internal::RewardsEngine>(
        _rewardsClient->MakeRemote(),
        [self handleFlags:luxxle_rewards::RewardsFlags::ForCurrentProcess()]);

    _rewardsDatabase.BindRemote<luxxle_rewards::internal::RewardsDatabase>(
        base::FilePath(base::SysNSStringToUTF8([self rewardsDatabasePath])));

    _creatorPrefixStore.BindRemote<luxxle_rewards::internal::HashPrefixStore>(
        base::FilePath(base::SysNSStringToUTF8([self creatorPrefixStorePath])));
  }
  return self;
}

- (void)dealloc {
  [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (luxxle_rewards::mojom::RewardsEngineOptions)handleFlags:
    (const luxxle_rewards::RewardsFlags&)flags {
  luxxle_rewards::mojom::RewardsEngineOptions options;
  if (flags.environment) {
    switch (*flags.environment) {
      case luxxle_rewards::RewardsFlags::Environment::kDevelopment:
        options.environment = luxxle_rewards::mojom::Environment::kDevelopment;
        break;
      case luxxle_rewards::RewardsFlags::Environment::kStaging:
        options.environment = luxxle_rewards::mojom::Environment::kStaging;
        break;
      case luxxle_rewards::RewardsFlags::Environment::kProduction:
        options.environment = luxxle_rewards::mojom::Environment::kProduction;
        break;
    }
  }

  if (flags.reconcile_interval) {
    options.reconcile_interval = *flags.reconcile_interval;
  }

  if (flags.retry_interval) {
    options.retry_interval = *flags.retry_interval;
  }

  return options;
}

- (void)postSelfTask:(void (^)(LuxxleRewardsAPI*))task {
  auto __weak weakSelf = self;
  _taskRunner->PostTask(FROM_HERE, base::BindOnce(^{
                          if (weakSelf) {
                            task(weakSelf);
                          }
                        }));
}

- (void)initializeRewardsService:(nullable void (^)())completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    [selfPtr initializeRewardsServiceInternal:completion];
  }];
}

- (void)initializeRewardsServiceInternal:(nullable void (^)())completion {
  if (self.initialized || self.initializing) {
    return;
  }
  self.initializing = YES;

  auto __weak weakSelf = self;
  auto callback = base::BindOnce(^(luxxle_rewards::mojom::Result result) {
    auto strongSelf = weakSelf;
    if (!strongSelf) {
      return;
    }
    strongSelf.initialized =
        (result == luxxle_rewards::mojom::Result::OK ||
         result == luxxle_rewards::mojom::Result::NO_LEGACY_STATE ||
         result == luxxle_rewards::mojom::Result::NO_PUBLISHER_STATE);
    strongSelf.initializing = NO;
    if (strongSelf.initialized) {
      [strongSelf getRewardsParameters:nil];
      [strongSelf fetchBalance:nil];
    } else {
      LLOG(0, @"Rewards Initialization Failed with error: %d",
           base::to_underlying(result));
    }
    strongSelf.initializationResult = static_cast<LuxxleRewardsResult>(result);
    if (completion) {
      completion();
    }
    for (RewardsObserver* observer in [strongSelf.observers copy]) {
      if (observer.walletInitalized) {
        observer.walletInitalized(strongSelf.initializationResult);
      }
    }
  });

  _rewardsEngine->Initialize(std::move(callback));
}

- (NSString*)rewardsDatabasePath {
  return [self.storagePath stringByAppendingPathComponent:@"Rewards.db"];
}

- (NSString*)creatorPrefixStorePath {
  return
      [self.storagePath stringByAppendingPathComponent:@"RewardsCreators.db"];
}

- (void)resetRewardsDatabase {
  const auto dbPath = [self rewardsDatabasePath];
  [NSFileManager.defaultManager removeItemAtPath:dbPath error:nil];
  [NSFileManager.defaultManager
      removeItemAtPath:[dbPath stringByAppendingString:@"-journal"]
                 error:nil];
  _rewardsDatabase.BindRemote<luxxle_rewards::internal::RewardsDatabase>(
      base::FilePath(base::SysNSStringToUTF8(dbPath)));

  const auto prefixPath = [self creatorPrefixStorePath];
  [NSFileManager.defaultManager removeItemAtPath:prefixPath error:nil];
  _creatorPrefixStore.BindRemote<luxxle_rewards::internal::HashPrefixStore>(
      base::FilePath(base::SysNSStringToUTF8(prefixPath)));
}

- (NSString*)randomStatePath {
  return
      [self.storagePath stringByAppendingPathComponent:@"random_state.plist"];
}

#pragma mark - Observers

- (void)addObserver:(RewardsObserver*)observer {
  [self.observers addObject:observer];
}

- (void)removeObserver:(RewardsObserver*)observer {
  [self.observers removeObject:observer];
}

#pragma mark - Wallet

- (void)createWallet:(void (^)(NSError* _Nullable))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    [selfPtr createWalletInternal:completion];
  }];
}

- (void)createWalletInternal:(void (^)(NSError* _Nullable))completion {
  using luxxle_rewards::mojom::CreateRewardsWalletResult;

  // Results that can come from CreateRewardsWallet():
  //   - OK: Good to go
  //   - ERROR: Already initialized
  //   - BAD_REGISTRATION_RESPONSE: Request credentials call failure or
  //   malformed data
  //   - REGISTRATION_VERIFICATION_FAILED: Missing master user token
  self.initializingWallet = YES;

  auto __weak weakSelf = self;

  auto callback = base::BindOnce(^(CreateRewardsWalletResult create_result) {
    auto strongSelf = weakSelf;
    if (!strongSelf) {
      return;
    }
    luxxle_rewards::mojom::Result result =
        create_result == CreateRewardsWalletResult::kSuccess
            ? luxxle_rewards::mojom::Result::OK
            : luxxle_rewards::mojom::Result::FAILED;

    NSError* error = nil;
    if (result != luxxle_rewards::mojom::Result::OK) {
      std::map<luxxle_rewards::mojom::Result, std::string> errorDescriptions{
          {luxxle_rewards::mojom::Result::FAILED,
           "The wallet was already initialized"},
          {luxxle_rewards::mojom::Result::BAD_REGISTRATION_RESPONSE,
           "Request credentials call failure or malformed data"},
          {luxxle_rewards::mojom::Result::REGISTRATION_VERIFICATION_FAILED,
           "Missing master user token from registered persona"},
      };
      NSDictionary* userInfo = @{};
      const auto description =
          errorDescriptions[static_cast<luxxle_rewards::mojom::Result>(result)];
      if (description.length() > 0) {
        userInfo =
            @{NSLocalizedDescriptionKey : base::SysUTF8ToNSString(description)};
      }
      error = [NSError errorWithDomain:LuxxleRewardsErrorDomain
                                  code:static_cast<NSInteger>(result)
                              userInfo:userInfo];
    }

    strongSelf.initializingWallet = NO;

    if (completion) {
      completion(error);
    }

    for (RewardsObserver* observer in [strongSelf.observers copy]) {
      if (observer.walletInitalized) {
        observer.walletInitalized(static_cast<LuxxleRewardsResult>(result));
      }
    }
  });

  _rewardsEngine->CreateRewardsWallet("", std::move(callback));
}

- (void)currentWalletInfo:
    (void (^)(LuxxleRewardsRewardsWallet* _Nullable wallet))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->GetRewardsWallet(
        base::BindOnce(^(luxxle_rewards::mojom::RewardsWalletPtr wallet) {
          const auto bridgedWallet = wallet.get() != nullptr
                                         ? [[LuxxleRewardsRewardsWallet alloc]
                                               initWithRewardsWallet:*wallet]
                                         : nil;
          completion(bridgedWallet);
        }));
  }];
}

- (void)getRewardsParameters:
    (void (^)(LuxxleRewardsRewardsParameters* _Nullable))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    auto __weak weakSelf = selfPtr;
    selfPtr->_rewardsEngine->GetRewardsParameters(
        base::BindOnce(^(luxxle_rewards::mojom::RewardsParametersPtr info) {
          auto strongSelf = weakSelf;
          if (!strongSelf) {
            return;
          }
          if (info) {
            strongSelf.rewardsParameters =
                [[LuxxleRewardsRewardsParameters alloc]
                    initWithRewardsParametersPtr:std::move(info)];
          } else {
            strongSelf.rewardsParameters = nil;
          }
          if (completion) {
            completion(strongSelf.rewardsParameters);
          }
        }));
  }];
}

- (void)fetchBalance:(void (^)(LuxxleRewardsBalance* _Nullable))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    auto __weak weakSelf = selfPtr;
    selfPtr->_rewardsEngine->FetchBalance(
        base::BindOnce(^(luxxle_rewards::mojom::BalancePtr balance) {
          auto strongSelf = weakSelf;
          if (!strongSelf) {
            return;
          }
          if (balance) {
            strongSelf.balance = [[LuxxleRewardsBalance alloc]
                initWithBalancePtr:std::move(balance)];
          }
          if (completion) {
            completion(strongSelf.balance);
          }
        }));
  }];
}

#pragma mark - Publishers

- (void)listActivityInfoFromStart:(unsigned int)start
                            limit:(unsigned int)limit
                           filter:(LuxxleRewardsActivityInfoFilter*)filter
                       completion:
                           (void (^)(NSArray<LuxxleRewardsPublisherInfo*>*))
                               completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    auto cppFilter = filter ? filter.cppObjPtr
                            : luxxle_rewards::mojom::ActivityInfoFilter::New();
    if (filter.excluded == LuxxleRewardsExcludeFilterFilterExcluded) {
      selfPtr->_rewardsEngine->GetExcludedList(base::BindOnce(
          ^(std::vector<luxxle_rewards::mojom::PublisherInfoPtr> list) {
            const auto publishers = NSArrayFromVector(
                &list, ^LuxxleRewardsPublisherInfo*(
                    const luxxle_rewards::mojom::PublisherInfoPtr& info) {
                  return [[LuxxleRewardsPublisherInfo alloc]
                      initWithPublisherInfo:*info];
                });
            completion(publishers);
          }));
    } else {
      selfPtr->_rewardsEngine->GetActivityInfoList(
          start, limit, std::move(cppFilter),
          base::BindOnce(
              ^(std::vector<luxxle_rewards::mojom::PublisherInfoPtr> list) {
                const auto publishers = NSArrayFromVector(
                    &list, ^LuxxleRewardsPublisherInfo*(
                        const luxxle_rewards::mojom::PublisherInfoPtr& info) {
                      return [[LuxxleRewardsPublisherInfo alloc]
                          initWithPublisherInfo:*info];
                    });
                completion(publishers);
              }));
    }
  }];
}

- (void)fetchPublisherActivityFromURL:(NSURL*)URL
                           faviconURL:(nullable NSURL*)faviconURL
                        publisherBlob:(nullable NSString*)publisherBlob
                                tabId:(uint64_t)tabId {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    if (!URL.absoluteString) {
      return;
    }

    GURL parsedUrl(base::SysNSStringToUTF8(URL.absoluteString));

    if (!parsedUrl.is_valid()) {
      return;
    }

    url::Origin origin = url::Origin::Create(parsedUrl);
    std::string baseDomain = GetDomainAndRegistry(
        origin.host(),
        net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);

    if (baseDomain == "") {
      return;
    }

    luxxle_rewards::mojom::VisitDataPtr visitData =
        luxxle_rewards::mojom::VisitData::New();
    visitData->domain = visitData->name = baseDomain;
    visitData->path = parsedUrl.PathForRequest();
    visitData->url = origin.Serialize();

    if (faviconURL.absoluteString) {
      visitData->favicon_url =
          base::SysNSStringToUTF8(faviconURL.absoluteString);
    }

    std::string blob = std::string();
    if (publisherBlob) {
      blob = base::SysNSStringToUTF8(publisherBlob);
    }

    selfPtr->_rewardsEngine->GetPublisherActivityFromUrl(
        tabId, std::move(visitData), blob);
  }];
}

- (void)refreshPublisherWithId:(NSString*)publisherId
                    completion:(void (^)(LuxxleRewardsPublisherStatus status))
                                   completion {
  if (self.loadingPublisherList) {
    completion(LuxxleRewardsPublisherStatusNotVerified);
    return;
  }
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->RefreshPublisher(
        base::SysNSStringToUTF8(publisherId),
        base::BindOnce(^(luxxle_rewards::mojom::PublisherStatus status) {
          completion(static_cast<LuxxleRewardsPublisherStatus>(status));
        }));
  }];
}

#pragma mark - Tips

- (void)listRecurringTips:
    (void (^)(NSArray<LuxxleRewardsPublisherInfo*>*))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->GetRecurringTips(base::BindOnce(
        ^(std::vector<luxxle_rewards::mojom::PublisherInfoPtr> list) {
          const auto publishers = NSArrayFromVector(
              &list, ^LuxxleRewardsPublisherInfo*(
                  const luxxle_rewards::mojom::PublisherInfoPtr& info) {
                return [[LuxxleRewardsPublisherInfo alloc]
                    initWithPublisherInfo:*info];
              });
          completion(publishers);
        }));
  }];
}

- (void)removeRecurringTipForPublisherWithId:(NSString*)publisherId {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->RemoveRecurringTip(
        base::SysNSStringToUTF8(publisherId), base::DoNothing());
  }];
}

#pragma mark - Reconcile

- (void)onReconcileComplete:(luxxle_rewards::mojom::Result)result
               contribution:
                   (luxxle_rewards::mojom::ContributionInfoPtr)contribution {
  // TODO we changed from probi to amount, so from string to double
  if (result == luxxle_rewards::mojom::Result::OK) {
    [self fetchBalance:nil];
  }
}

#pragma mark - Misc

- (void)rewardsInternalInfo:
    (void (^)(LuxxleRewardsRewardsInternalsInfo* _Nullable info))completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->GetRewardsInternalsInfo(
        base::BindOnce(^(luxxle_rewards::mojom::RewardsInternalsInfoPtr info) {
          auto bridgedInfo = info.get() != nullptr
                                 ? [[LuxxleRewardsRewardsInternalsInfo alloc]
                                       initWithRewardsInternalsInfo:*info.get()]
                                 : nil;
          completion(bridgedInfo);
        }));
  }];
}

- (void)allContributions:
    (void (^)(NSArray<LuxxleRewardsContributionInfo*>* contributions))
        completion {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->GetAllContributions(base::BindOnce(
        ^(std::vector<luxxle_rewards::mojom::ContributionInfoPtr> list) {
          const auto convetedList = NSArrayFromVector(
              &list, ^LuxxleRewardsContributionInfo*(
                  const luxxle_rewards::mojom::ContributionInfoPtr& info) {
                return [[LuxxleRewardsContributionInfo alloc]
                    initWithContributionInfo:*info];
              });
          completion(convetedList);
        }));
  }];
}

#pragma mark - Preferences

- (void)setMinimumVisitDuration:(int)minimumVisitDuration {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->SetPublisherMinVisitTime(minimumVisitDuration);
  }];
}

- (void)setMinimumNumberOfVisits:(int)minimumNumberOfVisits {
  [self postSelfTask:^(LuxxleRewardsAPI* selfPtr) {
    selfPtr->_rewardsEngine->SetPublisherMinVisits(minimumNumberOfVisits);
  }];
}

- (void)setUserPreferenceValue:(const std::string&)path
                         value:(base::Value)value
                      callback:
                          (RewardsEngineClient::SetUserPreferenceValueCallback)
                              callback {
  self.profilePrefService->Set(path, std::move(value));
  std::move(callback).Run();
}

- (void)userPreferenceValue:(const std::string&)path
                   callback:
                       (RewardsEngineClient::GetUserPreferenceValueCallback)
                           callback {
  std::move(callback).Run(self.profilePrefService->GetValue(path).Clone());
}

- (void)
    clearUserPreferenceValue:(const std::string&)path
                    callback:
                        (RewardsEngineClient::ClearUserPreferenceValueCallback)
                            callback {
  self.profilePrefService->ClearPref(path);
  std::move(callback).Run();
}

- (void)initProfilePrefService {
  std::vector<ProfileIOS*> profiles =
      GetApplicationContext()->GetProfileManager()->GetLoadedProfiles();
  CHECK(!profiles.empty());
  _profilePrefService = profiles.front()->GetPrefs();
  CHECK(_profilePrefService);
}

- (void)maybeMigrateProfilePrefs {
  // Migrates "leger_pref.plist" data into PrefService preferences.
  NSString* legacyProfilePrefsPath =
      [self.storagePath stringByAppendingPathComponent:@"ledger_pref.plist"];
  NSDictionary* legacyProfilePrefs = [[NSMutableDictionary alloc]
      initWithContentsOfFile:legacyProfilePrefsPath];
  if (!legacyProfilePrefs) {
    return;
  }

  LLOG(1, @"Migrating profile prefs");
  CHECK(self.profilePrefService);

  if ([legacyProfilePrefs objectForKey:@"wallets.luxxle"]) {
    auto json = (NSString*)legacyProfilePrefs[@"wallets.luxxle"];
    if (json) {
      self.profilePrefService->SetString(luxxle_rewards::prefs::kWalletLuxxle,
                                         base::SysNSStringToUTF8(json));
    }
  }

  if ([legacyProfilePrefs objectForKey:@"creation_stamp"]) {
    auto value = [legacyProfilePrefs[@"creation_stamp"] unsignedLongLongValue];
    self.profilePrefService->SetUint64(luxxle_rewards::prefs::kCreationStamp,
                                       value);
  }

  if ([legacyProfilePrefs objectForKey:@"publisher_prefix_list_stamp"]) {
    auto value = [legacyProfilePrefs[@"publisher_prefix_list_stamp"]
        unsignedLongLongValue];
    self.profilePrefService->SetUint64(
        luxxle_rewards::prefs::kServerPublisherListStamp, value);
  }

  NSError* error = nil;
  [[NSFileManager defaultManager] removeItemAtPath:legacyProfilePrefsPath
                                             error:&error];
  if (error) {
    LLOG(0, @"Failed to remove legacy prefs: %@", error);
  }
}

#pragma mark - Notifications

- (void)clearAllNotifications {
  // Not used on iOS
}

#pragma mark - Network

- (NSString*)customUserAgent {
  return self.commonOps.customUserAgent;
}

- (void)setCustomUserAgent:(NSString*)customUserAgent {
  self.commonOps.customUserAgent = [customUserAgent
      stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

- (void)loadUrl:(luxxle_rewards::mojom::UrlRequestPtr)request
       callback:(RewardsEngineClient::LoadURLCallback)callback {
  std::map<luxxle_rewards::mojom::UrlMethod, std::string> methodMap{
      {luxxle_rewards::mojom::UrlMethod::GET, "GET"},
      {luxxle_rewards::mojom::UrlMethod::POST, "POST"},
      {luxxle_rewards::mojom::UrlMethod::PUT, "PUT"},
      {luxxle_rewards::mojom::UrlMethod::DEL, "DELETE"}};

  if (!request) {
    request = luxxle_rewards::mojom::UrlRequest::New();
  }

  const auto copiedURL = base::SysUTF8ToNSString(request->url);

  auto cb = std::make_shared<decltype(callback)>(std::move(callback));
  return [self.commonOps
      loadURLRequest:request->url
             headers:request->headers
             content:request->content
        content_type:request->content_type
              method:methodMap[request->method]
            callback:^(
                const std::string& errorDescription, int statusCode,
                NSData* responseData,
                const base::flat_map<std::string, std::string>& headers) {
              std::string response;
              if (responseData && responseData.length > 0) {
                response =
                    std::string(static_cast<const char*>(responseData.bytes),
                                responseData.length);
              }

              auto url_response = luxxle_rewards::mojom::UrlResponse::New();
              url_response->url = base::SysNSStringToUTF8(copiedURL);
              url_response->error = errorDescription;
              url_response->status_code = statusCode;
              url_response->body = response;
              url_response->headers = headers;

              if (cb) {
                std::move(*cb).Run(std::move(url_response));
              }
            }];
}

- (void)fetchFavIcon:(const std::string&)url
          faviconKey:(const std::string&)faviconKey
            callback:(RewardsEngineClient::FetchFavIconCallback)callback {
  std::move(callback).Run(NO, std::string());
}

#pragma mark - Logging

- (void)log:(const std::string&)file
            line:(int32_t)line
    verboseLevel:(int32_t)verboseLevel
         message:(const std::string&)message {
  const int vlog_level =
      logging::GetVlogLevelHelper(file.c_str(), file.length());
  if (verboseLevel <= vlog_level) {
    logging::LogMessage(file.c_str(), line, -verboseLevel).stream() << message;
  }
}

#pragma mark - Publisher Database

- (void)publisherListNormalized:
    (std::vector<luxxle_rewards::mojom::PublisherInfoPtr>)list {
  // Not used on iOS
}

- (void)onPanelPublisherInfo:(luxxle_rewards::mojom::Result)result
               publisherInfo:
                   (luxxle_rewards::mojom::PublisherInfoPtr)publisher_info
                    windowId:(uint64_t)windowId {
  if (publisher_info.get() == nullptr ||
      result != luxxle_rewards::mojom::Result::OK) {
    return;
  }
  auto info =
      [[LuxxleRewardsPublisherInfo alloc] initWithPublisherInfo:*publisher_info];
  for (RewardsObserver* observer in [self.observers copy]) {
    if (observer.fetchedPanelPublisher) {
      observer.fetchedPanelPublisher(info, windowId);
    }
  }
}

- (void)onPublisherRegistryUpdated {
  // Not used on iOS
}

- (void)onPublisherUpdated:(const std::string&)publisherId {
  // Not used on iOS
}

- (void)showNotification:(const std::string&)type
                    args:(std::vector<std::string>)args
                callback:
                    (RewardsEngineClient::ShowNotificationCallback)callback {
  // Not used on iOS
}

- (void)reconcileStampReset {
  // Not used on iOS
}

- (void)runDbTransaction:(luxxle_rewards::mojom::DBTransactionPtr)transaction
                callback:
                    (RewardsEngineClient::RunDBTransactionCallback)callback {
  _rewardsDatabase->RunDBTransaction(std::move(transaction),
                                     std::move(callback));
}

- (void)
    updateCreatorPrefixStore:
        (luxxle_rewards::mojom::HashPrefixDataPtr)prefix_data
                    callback:
                        (RewardsEngineClient::UpdateCreatorPrefixStoreCallback)
                            callback {
  _creatorPrefixStore->UpdatePrefixes(std::move(prefix_data),
                                      std::move(callback));
}

- (void)creatorPrefixStoreContains:(const std::string&)value
                          callback:
                              (RewardsEngineClient::
                                   CreatorPrefixStoreContainsCallback)callback {
  _creatorPrefixStore->ContainsPrefix(value, std::move(callback));
}

- (void)walletDisconnected:(const std::string&)wallet_type {
  // Not used on iOS
}

- (void)deleteLog:(RewardsEngineClient::DeleteLogCallback)callback {
  std::move(callback).Run(luxxle_rewards::mojom::Result::OK);
}

- (void)encryptString:(const std::string&)value
             callback:(RewardsEngineClient::EncryptStringCallback)callback {
  std::string encrypted_value;
  if (!OSCrypt::EncryptString(value, &encrypted_value)) {
    std::move(callback).Run(std::nullopt);
    return;
  }
  std::move(callback).Run(std::make_optional(encrypted_value));
}

- (void)decryptString:(const std::string&)value
             callback:(RewardsEngineClient::DecryptStringCallback)callback {
  std::string decrypted_value;
  if (!OSCrypt::DecryptString(value, &decrypted_value)) {
    std::move(callback).Run(std::nullopt);
    return;
  }
  std::move(callback).Run(std::make_optional(decrypted_value));
}

- (void)externalWalletConnected {
  // Not used on iOS
}

- (void)externalWalletLoggedOut {
  // Not used on iOS
}

- (void)externalWalletReconnected {
  // Not used on iOS
}

- (void)externalWalletDisconnected {
  // Not used on iOS
}

@end

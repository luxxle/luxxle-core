/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "luxxle/ios/app/luxxle_core_main.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "base/allocator/partition_alloc_support.h"
#include "base/apple/bundle_locations.h"
#include "base/apple/foundation_util.h"
#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/i18n/icu_util.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/path_service.h"
#include "base/strings/sys_string_conversions.h"
#include "base/threading/thread_restrictions.h"
#include "luxxle/components/luxxle_user_agent/browser/luxxle_user_agent_exceptions.h"
#include "luxxle/components/p3a/buildflags.h"
#include "luxxle/components/p3a/histograms_luxxleizer.h"
#include "luxxle/components/p3a/p3a_config.h"
#include "luxxle/components/p3a/p3a_service.h"
#include "luxxle/ios/app/luxxle_main_delegate.h"
#include "luxxle/ios/app/luxxle_profile_controller+private.h"
#include "luxxle/ios/app/luxxle_profile_controller.h"
#include "luxxle/ios/browser/api/luxxle_shields/adblock_service+private.h"
#include "luxxle/ios/browser/api/luxxle_user_agent/luxxle_user_agent_exceptions_ios+private.h"
#include "luxxle/ios/browser/api/https_upgrade_exceptions/https_upgrade_exceptions_service+private.h"
#include "luxxle/ios/browser/api/p3a/luxxle_p3a_utils+private.h"
#include "luxxle/ios/browser/api/p3a/luxxle_p3a_utils.h"
#include "luxxle/ios/browser/application_context/luxxle_application_context_impl.h"
#include "luxxle/ios/browser/ui/webui/luxxle_web_ui_controller_factory.h"
#include "luxxle/ios/browser/web/luxxle_web_client.h"
#import "build/blink_buildflags.h"
#include "components/component_updater/component_updater_paths.h"
#include "ios/chrome/app/startup/provider_registration.h"
#include "ios/chrome/browser/shared/model/paths/paths.h"
#include "ios/chrome/browser/shared/model/prefs/pref_names.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "ios/chrome/browser/webui/ui_bundled/chrome_web_ui_ios_controller_factory.h"
#include "ios/public/provider/chrome/browser/overrides/overrides_api.h"
#include "ios/public/provider/chrome/browser/ui_utils/ui_utils_api.h"
#include "ios/web/public/init/web_main.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"

// Chromium logging is global, therefore we cannot link this to the instance in
// question
static LuxxleCoreLogHandler _Nullable _logHandler = nil;

const LuxxleCoreLogSeverity LuxxleCoreLogSeverityFatal = logging::LOGGING_FATAL;
const LuxxleCoreLogSeverity LuxxleCoreLogSeverityError = logging::LOGGING_ERROR;
const LuxxleCoreLogSeverity LuxxleCoreLogSeverityWarning =
    logging::LOGGING_WARNING;
const LuxxleCoreLogSeverity LuxxleCoreLogSeverityInfo = logging::LOGGING_INFO;
const LuxxleCoreLogSeverity LuxxleCoreLogSeverityVerbose =
    logging::LOGGING_VERBOSE;

@interface LuxxleCoreMain () {
  std::unique_ptr<LuxxleWebClient> _webClient;
  std::unique_ptr<LuxxleMainDelegate> _delegate;
  std::vector<std::string> _argv_store;
  std::unique_ptr<const char*[]> _raw_args;
  std::unique_ptr<web::WebMain> _webMain;
  scoped_refptr<p3a::P3AService> _p3a_service;
  scoped_refptr<p3a::HistogramsLuxxleizer> _histogram_luxxleizer;
}
@property(nonatomic) LuxxleProfileController* profileController;
@property(nonatomic) LuxxleP3AUtils* p3aUtils;
@property(nonatomic)
    HTTPSUpgradeExceptionsService* httpsUpgradeExceptionsService;
@property(nonatomic) LuxxleUserAgentExceptionsIOS* luxxleUserAgentExceptions;
@end

@implementation LuxxleCoreMain

- (instancetype)init {
  return [self initWithAdditionalSwitches:@[]];
}

- (instancetype)initWithAdditionalSwitches:
    (NSArray<LuxxleCoreSwitch*>*)additionalSwitches {
  if ((self = [super init])) {
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppEnterBackground:)
               name:UIApplicationDidEnterBackgroundNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppEnterForeground:)
               name:UIApplicationWillEnterForegroundNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onAppWillTerminate:)
               name:UIApplicationWillTerminateNotification
             object:nil];

    @autoreleasepool {
      ios::RegisterPathProvider();

      // Bundled components are not supported on ios, so DIR_USER_DATA is passed
      // for all three arguments.
      component_updater::RegisterPathProvider(
          ios::DIR_USER_DATA, ios::DIR_USER_DATA, ios::DIR_USER_DATA);
    }

#if PA_BUILDFLAG(USE_PARTITION_ALLOC) && !BUILDFLAG(USE_BLINK)
    // ContentMainRunnerImpl::Initialize calls this when USE_BLINK is true.
    base::allocator::PartitionAllocSupport::Get()->ReconfigureEarlyish("");
#endif  // PA_BUILDFLAG(USE_PARTITION_ALLOC) && !BUILDFLAG(USE_BLINK)

    NSBundle* baseBundle = base::apple::OuterBundle();
    base::apple::SetBaseBundleID(
        base::SysNSStringToUTF8([baseBundle bundleIdentifier]).c_str());

    // Register all providers before calling any Chromium code.
    [ProviderRegistration registerProviders];

    // Setup WebClient ([ClientRegistration registerClients])
    _webClient.reset(new LuxxleWebClient());
    web::SetWebClient(_webClient.get());

    _delegate.reset(new LuxxleMainDelegate());

    // Start Main ([ChromeMainStarter startChromeMain])
    web::WebMainParams params(_delegate.get());

    // Parse Switches, Features, Arguments (Command-Line Arguments)
    NSMutableArray* arguments =
        [[[NSProcessInfo processInfo] arguments] mutableCopy];
    NSMutableArray* switches = [[NSMutableArray alloc] init];
    for (LuxxleCoreSwitch* sv in additionalSwitches) {
      if (!sv.value) {
        [switches addObject:[NSString stringWithFormat:@"--%@", sv.key]];
      } else {
        [switches
            addObject:[NSString stringWithFormat:@"--%@=%@", sv.key, sv.value]];
      }
    }
    [arguments addObjectsFromArray:switches];
    params.argc = [arguments count];

    // Allocate memory to convert from iOS arguments to Native arguments
    _raw_args.reset(new const char*[params.argc]);
    _argv_store.resize([arguments count]);

    for (NSUInteger i = 0; i < [arguments count]; i++) {
      _argv_store[i] = base::SysNSStringToUTF8([arguments objectAtIndex:i]);
      _raw_args[i] = _argv_store[i].c_str();
    }
    params.argv = _raw_args.get();

    // Setup WebMain
    _webMain = std::make_unique<web::WebMain>(std::move(params));
    _webMain->Startup();

    // Initialize the provider UI global state.
    ios::provider::InitializeUI();

    // Setup WebUI (Sync Internals and other WebViews)
    web::WebUIIOSControllerFactory::RegisterFactory(
        LuxxleWebUIControllerFactory::GetInstance());

    // TODO(darkdh): move _adblockService and _backgroundImageService to
    // LuxxleWebMainParts::PreMainMessageLoopRun
    // https://github.com/luxxle/luxxle-browser/issues/40567
    component_updater::ComponentUpdateService* cus =
        GetApplicationContext()->GetComponentUpdateService();

    _adblockService = [[AdblockService alloc] initWithComponentUpdater:cus];
  }
  return self;
}

- (void)dealloc {
  _profileController = nil;

  _webMain.reset();
  _raw_args.reset();
  _argv_store = {};
  _delegate.reset();
  _webClient.reset();
}

- (void)onAppEnterBackground:(NSNotification*)notification {
  auto* context = GetApplicationContext();
  if (context) {
    context->OnAppEnterBackground();
    // Since we don't use the WebViewWebMainParts, local state is never commited
    // on app background
    context->GetLocalState()->CommitPendingWrite();
  }
}

- (void)onAppEnterForeground:(NSNotification*)notification {
  auto* context = GetApplicationContext();
  if (context)
    context->OnAppEnterForeground();
}

- (void)onAppWillTerminate:(NSNotification*)notification {
  // ApplicationContextImpl doesn't get teardown call at the moment because we
  // cannot dealloc this class yet without crashing.
  GetApplicationContext()->GetLocalState()->CommitPendingWrite();
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)setUserAgent:(NSString*)userAgent {
  _webClient->SetLegacyUserAgent(base::SysNSStringToUTF8(userAgent));
}

- (void)scheduleLowPriorityStartupTasks {
  // Install overrides
  ios::provider::InstallOverrides();

  // Make sure the system url request getter is called at least once during
  // startup in case cleanup is done early before first network request
  GetApplicationContext()->GetSystemURLRequestContext();
}

+ (void)setLogHandler:(LuxxleCoreLogHandler)logHandler {
  _logHandler = logHandler;
  logging::SetLogMessageHandler(&CustomLogHandler);
}

static bool CustomLogHandler(int severity,
                             const char* file,
                             int line,
                             size_t message_start,
                             const std::string& str) {
  if (!_logHandler) {
    return false;
  }
  if (severity > logging::LOGGING_VERBOSE ||
      severity <= logging::GetVlogLevelHelper(file, strlen(file))) {
    return _logHandler(severity, base::SysUTF8ToNSString(file), line,
                       message_start, base::SysUTF8ToNSString(str));
  }
  return true;
}

- (void)loadDefaultProfile:
    (void (^)(LuxxleProfileController*))completionHandler {
  // Initialize and set the main browser state.
  auto* localState = GetApplicationContext()->GetLocalState();
  auto* profileManager = GetApplicationContext()->GetProfileManager();
  std::string profileName =
      "Default";  // kIOSChromeInitialProfile which is now removed
  // Set this as the last used profile always so that its saved for the future
  // where we may have multiple profile support and need to read it from local
  // state before creating the profile
  localState->SetString(prefs::kLastUsedProfile, profileName);
  profileManager->CreateProfileAsync(
      profileName, base::BindOnce(^(ProfileIOS* profile) {
        [self profileLoaded:profile completionHandler:completionHandler];
      }));
}

- (void)profileLoaded:(ProfileIOS*)profile
    completionHandler:(void (^)(LuxxleProfileController*))completionHandler {
  CHECK(profile) << "A default profile must be loaded.";
  self.profileController =
      [[LuxxleProfileController alloc] initWithProfile:profile];
  completionHandler(self.profileController);
}

#pragma mark -

- (HTTPSUpgradeExceptionsService*)httpsUpgradeExceptionsService {
  if (!_httpsUpgradeExceptionsService) {
    _httpsUpgradeExceptionsService =
        [[HTTPSUpgradeExceptionsService alloc] init];
  }
  return _httpsUpgradeExceptionsService;
}

- (LuxxleUserAgentExceptionsIOS*)luxxleUserAgentExceptions {
  if (!_luxxleUserAgentExceptions) {
    luxxle_user_agent::LuxxleUserAgentExceptions* luxxle_user_agent_exceptions =
        luxxle_user_agent::LuxxleUserAgentExceptions::GetInstance();
    if (!luxxle_user_agent_exceptions) {
      return nil;
    }
    _luxxleUserAgentExceptions = [[LuxxleUserAgentExceptionsIOS alloc]
        initWithLuxxleUserAgentExceptions:luxxle_user_agent_exceptions];
  }
  return _luxxleUserAgentExceptions;
}

- (void)initializeP3AServiceForChannel:(NSString*)channel
                      installationDate:(NSDate*)installDate {
#if BUILDFLAG(LUXXLE_P3A_ENABLED)
  _p3a_service = base::MakeRefCounted<p3a::P3AService>(
      *GetApplicationContext()->GetLocalState(),
      base::SysNSStringToUTF8(channel), base::Time::FromNSDate(installDate),
      p3a::P3AConfig::LoadFromCommandLine());
  _p3a_service->InitCallbacks();
  _p3a_service->Init(GetApplicationContext()->GetSharedURLLoaderFactory());
  _histogram_luxxleizer = p3a::HistogramsLuxxleizer::Create();
#endif  // BUILDFLAG(LUXXLE_P3A_ENABLED)
}

- (LuxxleP3AUtils*)p3aUtils {
  if (!_p3aUtils) {
    _p3aUtils = [[LuxxleP3AUtils alloc]
        initWithLocalState:GetApplicationContext()->GetLocalState()
                p3aService:_p3a_service];
  }
  return _p3aUtils;
}

+ (bool)initializeICUForTesting {
  NSBundle* bundle = [NSBundle bundleForClass:self];
  base::apple::SetOverrideOuterBundle(bundle);
  base::apple::SetOverrideFrameworkBundle(bundle);
  return base::i18n::InitializeICU();
}

+ (void)initializeResourceBundleForTesting {
  @autoreleasepool {
    ios::RegisterPathProvider();
    ui::RegisterPathProvider();
  }

  base::AtExitManager exit_manager;
  base::CommandLine::Init(0, nullptr);

  [LuxxleCoreMain initializeICUForTesting];

  NSBundle* baseBundle = base::apple::OuterBundle();
  base::apple::SetBaseBundleID(
      base::SysNSStringToUTF8([baseBundle bundleIdentifier]).c_str());

  // Register all providers before calling any Chromium code.
  [ProviderRegistration registerProviders];

  ui::ResourceBundle::InitSharedInstanceWithLocale(
      "en-US", nullptr, ui::ResourceBundle::LOAD_COMMON_RESOURCES);

  // Add Luxxle Resource Pack
  base::FilePath luxxle_pack_path;
  base::PathService::Get(base::DIR_ASSETS, &luxxle_pack_path);
  luxxle_pack_path = luxxle_pack_path.AppendASCII("luxxle_resources.pak");
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      luxxle_pack_path, ui::kScaleFactorNone);
}

@end

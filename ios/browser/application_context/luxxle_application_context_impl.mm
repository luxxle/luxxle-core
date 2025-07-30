// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/application_context/luxxle_application_context_impl.h"

#include <string>

#import "base/command_line.h"
#import "base/task/sequenced_task_runner.h"
#include "base/task/single_thread_task_runner.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_component.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_component_updater_delegate.h"
#include "luxxle/components/luxxle_component_updater/browser/local_data_files_service.h"
#include "luxxle/components/luxxle_sync/network_time_helper.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/debounce/core/browser/debounce_component_installer.h"
#include "luxxle/components/https_upgrade_exceptions/browser/https_upgrade_exceptions_service.h"
#include "luxxle/components/url_sanitizer/browser/url_sanitizer_component_installer.h"
#include "luxxle/ios/browser/luxxle_wallet/wallet_data_files_installer_delegate_impl.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "net/base/features.h"

LuxxleApplicationContextImpl::LuxxleApplicationContextImpl(
    base::SequencedTaskRunner* local_state_task_runner,
    const base::CommandLine& command_line,
    const std::string& locale,
    const std::string& country)
    : ApplicationContextImpl(local_state_task_runner,
                             command_line,
                             locale,
                             country) {}

inline LuxxleApplicationContextImpl::~LuxxleApplicationContextImpl() = default;

// MARK: - ApplicationContextImpl

ukm::UkmRecorder* LuxxleApplicationContextImpl::GetUkmRecorder() {
  return nullptr;
}

gcm::GCMDriver* LuxxleApplicationContextImpl::GetGCMDriver() {
  return nullptr;
}

// MARK: - LuxxleApplicationContextImpl

luxxle_component_updater::LuxxleComponent::Delegate*
LuxxleApplicationContextImpl::luxxle_component_updater_delegate() {
  if (!luxxle_component_updater_delegate_) {
    luxxle_component_updater_delegate_ = std::make_unique<
        luxxle_component_updater::LuxxleComponentUpdaterDelegate>(
        GetComponentUpdateService(), GetLocalState(), GetApplicationLocale());
  }

  return luxxle_component_updater_delegate_.get();
}

luxxle_component_updater::LocalDataFilesService*
LuxxleApplicationContextImpl::local_data_files_service() {
  if (!local_data_files_service_) {
    local_data_files_service_ =
        luxxle_component_updater::LocalDataFilesServiceFactory(
            luxxle_component_updater_delegate());
  }
  return local_data_files_service_.get();
}

luxxle::URLSanitizerComponentInstaller*
LuxxleApplicationContextImpl::url_sanitizer_component_installer() {
  if (!url_sanitizer_component_installer_) {
    url_sanitizer_component_installer_ =
        std::make_unique<luxxle::URLSanitizerComponentInstaller>(
            local_data_files_service());
  }
  return url_sanitizer_component_installer_.get();
}

debounce::DebounceComponentInstaller*
LuxxleApplicationContextImpl::debounce_component_installer() {
  if (!debounce_component_installer_) {
    debounce_component_installer_ =
        std::make_unique<debounce::DebounceComponentInstaller>(
            local_data_files_service());
  }
  return debounce_component_installer_.get();
}

https_upgrade_exceptions::HttpsUpgradeExceptionsService*
LuxxleApplicationContextImpl::https_upgrade_exceptions_service() {
  if (!https_upgrade_exceptions_service_) {
    https_upgrade_exceptions_service_ =
        https_upgrade_exceptions::HttpsUpgradeExceptionsServiceFactory(
            local_data_files_service());
  }
  return https_upgrade_exceptions_service_.get();
}

void LuxxleApplicationContextImpl::StartLuxxleServices() {
  // We need to Initialize the component installers
  // before calling Start on the local_data_files_service
  url_sanitizer_component_installer();
  debounce_component_installer();

  if (base::FeatureList::IsEnabled(net::features::kLuxxleHttpsByDefault)) {
    https_upgrade_exceptions_service();
  }

  // Start the local data file service
  local_data_files_service()->Start();

  luxxle_sync::NetworkTimeHelper::GetInstance()->SetNetworkTimeTracker(
      GetNetworkTimeTracker(),
      base::SingleThreadTaskRunner::GetCurrentDefault());

  luxxle_wallet::WalletDataFilesInstaller::GetInstance().SetDelegate(
      std::make_unique<luxxle_wallet::WalletDataFilesInstallerDelegateImpl>());
}

// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_APPLICATION_CONTEXT_LUXXLE_APPLICATION_CONTEXT_IMPL_H_
#define LUXXLE_IOS_BROWSER_APPLICATION_CONTEXT_LUXXLE_APPLICATION_CONTEXT_IMPL_H_

#include <memory>
#include <string>

#include "luxxle/components/luxxle_component_updater/browser/luxxle_component.h"
#include "luxxle/components/debounce/core/browser/debounce_component_installer.h"
#include "luxxle/components/https_upgrade_exceptions/browser/https_upgrade_exceptions_service.h"
#include "luxxle/components/url_sanitizer/browser/url_sanitizer_component_installer.h"
#include "ios/chrome/browser/application_context/model/application_context_impl.h"

namespace base {
class CommandLine;
class SequencedTaskRunner;
}  // namespace base

/// This extends the behaviors of the ApplicationContext
class LuxxleApplicationContextImpl : public ApplicationContextImpl {
 public:
  // Out-of-line constructor declaration
  LuxxleApplicationContextImpl(
      base::SequencedTaskRunner* local_state_task_runner,
      const base::CommandLine& command_line,
      const std::string& locale,
      const std::string& country);

  LuxxleApplicationContextImpl(const LuxxleApplicationContextImpl&) = delete;
  LuxxleApplicationContextImpl& operator=(const LuxxleApplicationContextImpl&) =
      delete;

  ~LuxxleApplicationContextImpl() override;

  // ApplicationContextImpl
  ukm::UkmRecorder* GetUkmRecorder() override;
  gcm::GCMDriver* GetGCMDriver() override;

  // LuxxleApplicationContextImpl
  luxxle::URLSanitizerComponentInstaller* url_sanitizer_component_installer();
  debounce::DebounceComponentInstaller* debounce_component_installer();
  https_upgrade_exceptions::HttpsUpgradeExceptionsService*
  https_upgrade_exceptions_service();

  // Start any services that we may need later
  void StartLuxxleServices();

 private:
  luxxle_component_updater::LuxxleComponent::Delegate*
  luxxle_component_updater_delegate();
  luxxle_component_updater::LocalDataFilesService* local_data_files_service();

  std::unique_ptr<luxxle_component_updater::LuxxleComponent::Delegate>
      luxxle_component_updater_delegate_;
  std::unique_ptr<luxxle_component_updater::LocalDataFilesService>
      local_data_files_service_;
  std::unique_ptr<luxxle::URLSanitizerComponentInstaller>
      url_sanitizer_component_installer_;
  std::unique_ptr<debounce::DebounceComponentInstaller>
      debounce_component_installer_;
  std::unique_ptr<https_upgrade_exceptions::HttpsUpgradeExceptionsService>
      https_upgrade_exceptions_service_;
};

#endif  // LUXXLE_IOS_BROWSER_APPLICATION_CONTEXT_LUXXLE_APPLICATION_CONTEXT_IMPL_H_

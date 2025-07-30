// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/web/luxxle_web_main_parts.h"

#include "base/command_line.h"
#include "base/path_service.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "luxxle/components/ai_chat/core/browser/local_models_updater.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_on_demand_updater.h"
#include "luxxle/components/luxxle_user_agent/browser/luxxle_user_agent_component_installer.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/ios/browser/application_context/luxxle_application_context_impl.h"
#include "chrome/browser/component_updater/zxcvbn_data_component_installer.h"
#include "components/component_updater/installer_policies/safety_tips_component_installer.h"
#include "ios/chrome/browser/application_context/model/application_context_impl.h"
#include "ios/chrome/browser/shared/model/paths/paths.h"
#include "ui/base/l10n/l10n_util_mac.h"
#include "ui/base/resource/resource_bundle.h"

namespace {
void RegisterComponentsForUpdate(
    component_updater::ComponentUpdateService* cus) {
  RegisterSafetyTipsComponent(cus);
  luxxle_wallet::WalletDataFilesInstaller::GetInstance()
      .MaybeRegisterWalletDataFilesComponent(
          cus, GetApplicationContext()->GetLocalState());
  ai_chat::MigrateDeprecatedLocalModelsComponent(cus);
  luxxle_user_agent::RegisterLuxxleUserAgentComponent(cus);
  RegisterZxcvbnDataComponent(cus);
}
}  // namespace

LuxxleWebMainParts::LuxxleWebMainParts(
    const base::CommandLine& parsed_command_line)
    : IOSChromeMainParts(parsed_command_line) {}

LuxxleWebMainParts::~LuxxleWebMainParts() {}

void LuxxleWebMainParts::PreCreateMainMessageLoop() {
  IOSChromeMainParts::PreCreateMainMessageLoop();

  // Add Luxxle Resource Pack
  auto luxxle_pack_path = base::PathService::CheckedGet(base::DIR_ASSETS);
  luxxle_pack_path = luxxle_pack_path.AppendASCII("luxxle_resources.pak");
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      luxxle_pack_path, ui::kScaleFactorNone);
}

void LuxxleWebMainParts::PreMainMessageLoopRun() {
  IOSChromeMainParts::PreMainMessageLoopRun();

  // Setup Component Updater
  component_updater::ComponentUpdateService* cus =
      application_context_->GetComponentUpdateService();
  DCHECK(cus);
  luxxle_component_updater::LuxxleOnDemandUpdater::GetInstance()
      ->RegisterOnDemandUpdater(&cus->GetOnDemandUpdater());
  RegisterComponentsForUpdate(cus);

  static_cast<LuxxleApplicationContextImpl*>(application_context_.get())
      ->StartLuxxleServices();
}

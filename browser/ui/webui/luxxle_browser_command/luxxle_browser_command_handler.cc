/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// based on //chrome/browser/ui/webui/browser_command/browser_command_handler.cc

#include "luxxle/browser/ui/webui/luxxle_browser_command/luxxle_browser_command_handler.h"

#include "base/containers/contains.h"
#include "luxxle/browser/ai_chat/ai_chat_service_factory.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/components/luxxle_education/education_urls.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#endif

namespace {

bool CanShowWalletOnboarding(Profile* profile) {
  return luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
             profile) != nullptr;
}

bool CanShowRewardsOnboarding(Profile* profile) {
  return luxxle_rewards::RewardsServiceFactory::GetForProfile(profile) !=
         nullptr;
}

bool CanShowVPNBubble(Profile* profile) {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  return luxxle_vpn::IsAllowedForContext(profile);
#else
  return false;
#endif
}

bool CanShowAIChat(Profile* profile) {
  return ai_chat::AIChatServiceFactory::GetForBrowserContext(profile) !=
         nullptr;
}

}  // namespace

LuxxleBrowserCommandHandler::LuxxleBrowserCommandHandler(
    mojo::PendingReceiver<
        luxxle_browser_command::mojom::LuxxleBrowserCommandHandler>
        pending_page_handler,
    Profile* profile,
    std::vector<luxxle_browser_command::mojom::Command> supported_commands,
    std::unique_ptr<Delegate> delegate)
    : profile_(profile),
      supported_commands_(supported_commands),
      delegate_(std::move(delegate)),
      page_handler_(this, std::move(pending_page_handler)) {}

LuxxleBrowserCommandHandler::~LuxxleBrowserCommandHandler() = default;

void LuxxleBrowserCommandHandler::CanExecuteCommand(
    luxxle_browser_command::mojom::Command command_id,
    CanExecuteCommandCallback callback) {
  if (!base::Contains(supported_commands_, command_id)) {
    std::move(callback).Run(false);
    return;
  }

  bool can_execute = false;
  switch (command_id) {
    case luxxle_browser_command::mojom::Command::kOpenWalletOnboarding:
      can_execute = CanShowWalletOnboarding(profile_);
      break;
    case luxxle_browser_command::mojom::Command::kOpenRewardsOnboarding:
      can_execute = CanShowRewardsOnboarding(profile_);
      break;
    case luxxle_browser_command::mojom::Command::kOpenVPNOnboarding:
      can_execute = CanShowVPNBubble(profile_);
      break;
    case luxxle_browser_command::mojom::Command::kOpenAIChat:
      can_execute = CanShowAIChat(profile_);
      break;
  }
  std::move(callback).Run(can_execute);
}

void LuxxleBrowserCommandHandler::ExecuteCommand(
    luxxle_browser_command::mojom::Command command_id,
    ExecuteCommandCallback callback) {
  if (!base::Contains(supported_commands_, command_id)) {
    std::move(callback).Run(false);
    return;
  }

  switch (command_id) {
    case luxxle_browser_command::mojom::Command::kOpenWalletOnboarding:
      delegate_->OpenURL(GURL(kLuxxleUIWalletURL),
                         WindowOpenDisposition::NEW_FOREGROUND_TAB);
      break;
    case luxxle_browser_command::mojom::Command::kOpenRewardsOnboarding:
      delegate_->OpenRewardsPanel();
      break;
    case luxxle_browser_command::mojom::Command::kOpenVPNOnboarding:
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
      delegate_->OpenVPNPanel();
      break;
#else
      std::move(callback).Run(false);
      return;
#endif
    case luxxle_browser_command::mojom::Command::kOpenAIChat:
      delegate_->OpenAIChat();
      break;
  }

  std::move(callback).Run(true);
}

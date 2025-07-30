/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string>
#include <vector>

#include "base/test/bind.h"
#include "base/test/test_future.h"
#include "luxxle/browser/ui/webui/luxxle_browser_command/luxxle_browser_command_handler.h"
#include "luxxle/components/luxxle_education/education_urls.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "content/public/test/browser_task_environment.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace luxxle_education {

class TestDelegate : public LuxxleBrowserCommandHandler::Delegate {
 public:
  using AddActionCallback = base::RepeatingCallback<void(std::string)>;

  explicit TestDelegate(AddActionCallback add_action)
      : add_action_(std::move(add_action)) {}

  ~TestDelegate() override = default;

  void OpenURL(const GURL& url, WindowOpenDisposition disposition) override {
    add_action_.Run("open-url: " + url.spec());
  }

  void OpenRewardsPanel() override { add_action_.Run("open-rewards-panel"); }
  void OpenVPNPanel() override { add_action_.Run("open-vpn-panel"); }
  void OpenAIChat() override { add_action_.Run("open-ai-chat"); }

 private:
  AddActionCallback add_action_;
};

class LuxxleEducationPageHandlerTest : public testing::Test {
 protected:
  void SetUp() override {
    TestingProfile::Builder builder;
    profile_ = builder.Build();
  }

  mojo::Remote<luxxle_browser_command::mojom::LuxxleBrowserCommandHandler>&
  CreateHandler(Profile* profile = nullptr) {
    auto delegate = std::make_unique<TestDelegate>(base::BindLambdaForTesting(
        [this](std::string action) { actions_.push_back(std::move(action)); }));

    if (!profile) {
      profile = profile_.get();
    }

    std::vector<luxxle_browser_command::mojom::Command> supported_commands = {};
    supported_commands.insert(
        supported_commands.end(),
        {luxxle_browser_command::mojom::Command::kOpenRewardsOnboarding,
         luxxle_browser_command::mojom::Command::kOpenWalletOnboarding,
         luxxle_browser_command::mojom::Command::kOpenVPNOnboarding,
         luxxle_browser_command::mojom::Command::kOpenAIChat});

    page_handler_ = std::make_unique<LuxxleBrowserCommandHandler>(
        remote_.BindNewPipeAndPassReceiver(), profile, supported_commands,
        std::move(delegate));

    return remote_;
  }

  Profile& profile() { return *profile_; }

  const std::vector<std::string>& actions() { return actions_; }

 private:
  content::BrowserTaskEnvironment task_environment_;
  ScopedTestingLocalState local_state_{TestingBrowserProcess::GetGlobal()};
  mojo::Remote<luxxle_browser_command::mojom::LuxxleBrowserCommandHandler>
      remote_;
  std::unique_ptr<Profile> profile_;
  std::unique_ptr<LuxxleBrowserCommandHandler> page_handler_;
  std::vector<std::string> actions_;
};

TEST_F(LuxxleEducationPageHandlerTest, BasicCommandsExecuted) {
  auto& handler = CreateHandler();

  base::test::TestFuture<bool> future;

  handler->ExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenWalletOnboarding,
      base::DoNothing());
  handler->ExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenRewardsOnboarding,
      future.GetCallback());

  ASSERT_TRUE(future.Get());
  EXPECT_EQ(actions()[0], "open-url: chrome://wallet/");
  EXPECT_EQ(actions()[1], "open-rewards-panel");
}

TEST_F(LuxxleEducationPageHandlerTest, VPNCommandsExecuted) {
  auto& handler = CreateHandler();
  base::test::TestFuture<bool> future;

  handler->ExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenVPNOnboarding,
      future.GetCallback());

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  ASSERT_TRUE(future.Get());
  EXPECT_EQ(actions()[0], "open-vpn-panel");
#else
  ASSERT_FALSE(future.Get());
  EXPECT_TRUE(actions().empty());
#endif
}

TEST_F(LuxxleEducationPageHandlerTest, ChatCommandsExecuted) {
  auto& handler = CreateHandler();
  base::test::TestFuture<bool> future;

  handler->ExecuteCommand(luxxle_browser_command::mojom::Command::kOpenAIChat,
                          future.GetCallback());

  ASSERT_TRUE(future.Get());
  EXPECT_EQ(actions()[0], "open-ai-chat");
}

TEST_F(LuxxleEducationPageHandlerTest, OffTheRecordProfile) {
  auto* otr_profile = profile().GetOffTheRecordProfile(
      Profile::OTRProfileID::CreateUniqueForTesting(),
      /*create_if_needed=*/true);

  auto& handler = CreateHandler(otr_profile);

  base::test::TestFuture<bool> future;

  // browser/resources/luxxle_education/luxxle_education_app.ts
  // calls `CanExecuteCommand` before calling `ExecuteCommand`
  //
  // Since OTR does not allow, callback is immediately called w/ false
  handler->CanExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenWalletOnboarding,
      future.GetCallback());
  ASSERT_FALSE(future.Get());

  handler->CanExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenRewardsOnboarding,
      future.GetCallback());
  ASSERT_FALSE(future.Get());

  handler->CanExecuteCommand(
      luxxle_browser_command::mojom::Command::kOpenVPNOnboarding,
      future.GetCallback());
  ASSERT_FALSE(future.Get());

  handler->CanExecuteCommand(luxxle_browser_command::mojom::Command::kOpenAIChat,
                             future.GetCallback());
  ASSERT_FALSE(future.Get());

  EXPECT_TRUE(actions().empty());
}

}  // namespace luxxle_education

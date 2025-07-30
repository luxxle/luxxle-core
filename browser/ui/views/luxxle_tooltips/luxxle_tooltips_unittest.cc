/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip.h"
#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip_attributes.h"
#include "luxxle/browser/ui/views/luxxle_tooltips/luxxle_tooltip_popup.h"
#include "chrome/test/views/chrome_views_test_base.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "ui/events/base_event_utils.h"
#include "ui/views/controls/button/button.h"

class MockLuxxleTooltipDelegate : public luxxle_tooltips::LuxxleTooltipDelegate {
 public:
  MockLuxxleTooltipDelegate() {
    ON_CALL(*this, OnTooltipWidgetDestroyed).WillByDefault([this] {
      run_loop_.Quit();
    });
  }

  MOCK_METHOD1(OnTooltipShow, void(const std::string&));
  MOCK_METHOD1(OnTooltipClose, void(const std::string&));
  MOCK_METHOD1(OnTooltipWidgetDestroyed, void(const std::string&));
  MOCK_METHOD1(OnTooltipOkButtonPressed, void(const std::string&));
  MOCK_METHOD1(OnTooltipCancelButtonPressed, void(const std::string&));

  void WaitForWidgetDestroyedNotification() { run_loop_.Run(); }

  base::WeakPtr<LuxxleTooltipDelegate> AsWeakPtr() override {
    return weak_ptr_factory_.GetWeakPtr();
  }

 private:
  base::RunLoop run_loop_;
  base::WeakPtrFactory<LuxxleTooltipDelegate> weak_ptr_factory_{this};
};

class LuxxleTooltipsTest : public ChromeViewsTestBase {
 public:
  LuxxleTooltipsTest() = default;

 protected:
  std::unique_ptr<luxxle_tooltips::LuxxleTooltipPopup> CreateTooltipPopup(
      const std::string& id,
      const luxxle_tooltips::LuxxleTooltipAttributes& attributes) {
    auto tooltip = std::make_unique<luxxle_tooltips::LuxxleTooltip>(
        id, attributes, mock_tooltip_delegate_.AsWeakPtr());
    return std::make_unique<luxxle_tooltips::LuxxleTooltipPopup>(
        std::move(tooltip));
  }

  void ClickButton(views::Button* button) const {
    ui::MouseEvent press_event(ui::EventType::kMousePressed, gfx::Point(1, 1),
                               gfx::Point(), ui::EventTimeForNow(),
                               ui::EF_LEFT_MOUSE_BUTTON, 0);
    button->OnMousePressed(press_event);
    ui::MouseEvent release_event(
        ui::EventType::kMouseReleased, gfx::Point(1, 1), gfx::Point(),
        ui::EventTimeForNow(), ui::EF_LEFT_MOUSE_BUTTON, 0);
    button->OnMouseReleased(release_event);
  }

  testing::NiceMock<MockLuxxleTooltipDelegate> mock_tooltip_delegate_;
};

TEST_F(LuxxleTooltipsTest, OkButtonPressed) {
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipShow(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipOkButtonPressed(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipClose(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipWidgetDestroyed(testing::_));

  auto tooltip_popup = CreateTooltipPopup(
      "id", luxxle_tooltips::LuxxleTooltipAttributes(u"Title", u"Body", u"OK"));
  ASSERT_TRUE(tooltip_popup);

  tooltip_popup->Show();

  ClickButton(tooltip_popup->ok_button_for_testing());

  mock_tooltip_delegate_.WaitForWidgetDestroyedNotification();

  tooltip_popup.release();
}

TEST_F(LuxxleTooltipsTest, CancelButtonPressed) {
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipShow(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipCancelButtonPressed(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipClose(testing::_));
  EXPECT_CALL(mock_tooltip_delegate_, OnTooltipWidgetDestroyed(testing::_));

  auto tooltip_popup =
      CreateTooltipPopup("id", luxxle_tooltips::LuxxleTooltipAttributes(
                                   u"Title", u"Body", u"OK", u"Cancel"));
  ASSERT_TRUE(tooltip_popup);

  tooltip_popup->Show();

  ClickButton(tooltip_popup->cancel_button_for_testing());

  mock_tooltip_delegate_.WaitForWidgetDestroyedNotification();

  tooltip_popup.release();
}

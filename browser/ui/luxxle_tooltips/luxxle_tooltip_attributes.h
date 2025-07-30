/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_ATTRIBUTES_H_
#define LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_ATTRIBUTES_H_

#include <string>

namespace luxxle_tooltips {

class LuxxleTooltipAttributes {
 public:
  LuxxleTooltipAttributes(const std::u16string& title,
                         const std::u16string& body,
                         const std::u16string& ok_button_text,
                         const std::u16string& cancel_button_text = u"");
  ~LuxxleTooltipAttributes();

  LuxxleTooltipAttributes(const LuxxleTooltipAttributes& other);
  LuxxleTooltipAttributes& operator=(const LuxxleTooltipAttributes& other);

  const std::u16string& title() const { return title_; }
  void set_title(const std::u16string& title) { title_ = title; }

  const std::u16string& body() const { return body_; }
  void set_body(const std::u16string& body) { body_ = body; }

  const std::u16string& ok_button_text() const { return ok_button_text_; }
  void set_ok_button_text(const std::u16string& ok_button_text) {
    ok_button_text_ = ok_button_text;
  }

  const std::u16string& cancel_button_text() const {
    return cancel_button_text_;
  }
  void set_cancel_button_text(const std::u16string& cancel_button_text) {
    cancel_button_text_ = cancel_button_text;
  }

  bool cancel_button_enabled() const { return cancel_button_enabled_; }
  void set_cancel_button_enabled(bool cancel_button_enabled) {
    cancel_button_enabled_ = cancel_button_enabled;
  }

 private:
  std::u16string title_;
  std::u16string body_;
  std::u16string ok_button_text_;
  std::u16string cancel_button_text_;
  bool cancel_button_enabled_ = false;
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_ATTRIBUTES_H_

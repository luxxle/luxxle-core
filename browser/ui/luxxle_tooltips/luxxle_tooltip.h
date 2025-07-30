/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_H_
#define LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_H_

#include <string>
#include <utility>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip_attributes.h"
#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip_delegate.h"

namespace luxxle_tooltips {

class LuxxleTooltip {
 public:
  // Create a new tooltip with an |id| and |attributes|.  |delegate|
  // will influence the behaviour of this tooltip and receives events
  // on its behalf. The delegate may be omitted
  LuxxleTooltip(const std::string& id,
               const LuxxleTooltipAttributes& attributes,
               base::WeakPtr<LuxxleTooltipDelegate> delegate);
  virtual ~LuxxleTooltip();

  LuxxleTooltip(const LuxxleTooltip&) = delete;
  LuxxleTooltip& operator=(const LuxxleTooltip&) = delete;

  const std::string& id() const { return id_; }

  const LuxxleTooltipAttributes& attributes() const { return attributes_; }
  void set_attributes(const LuxxleTooltipAttributes& attributes) {
    attributes_ = attributes;
  }

  std::u16string accessible_name() const;

  LuxxleTooltipDelegate* delegate() const { return delegate_.get(); }

  void set_delegate(base::WeakPtr<LuxxleTooltipDelegate> delegate) {
    DCHECK(!delegate_);
    delegate_ = std::move(delegate);
  }

  virtual void PerformOkButtonAction() {}
  virtual void PerformCancelButtonAction() {}

 protected:
  std::string id_;
  LuxxleTooltipAttributes attributes_;

 private:
  base::WeakPtr<LuxxleTooltipDelegate> delegate_;
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_H_

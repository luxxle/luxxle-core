/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/toolbar/ai_chat_button.h"

#include <memory>

#include "luxxle/browser/misc_metrics/profile_misc_metrics_service.h"
#include "luxxle/browser/misc_metrics/profile_misc_metrics_service_factory.h"
#include "luxxle/browser/ui/luxxle_pages.h"
#include "luxxle/components/ai_chat/core/browser/ai_chat_metrics.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
#include "luxxle/components/vector_icons/vector_icons.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "components/prefs/pref_service.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/menus/simple_menu_model.h"
#include "ui/views/accessibility/view_accessibility.h"

namespace {

class AIChatButtonMenuModel : public ui::SimpleMenuModel,
                              public ui::SimpleMenuModel::Delegate {
 public:
  explicit AIChatButtonMenuModel(PrefService* prefs)
      : ui::SimpleMenuModel(this), prefs_(*prefs) {
    Build();
  }

  AIChatButtonMenuModel(const AIChatButtonMenuModel&) = delete;
  AIChatButtonMenuModel& operator=(const AIChatButtonMenuModel&) = delete;
  ~AIChatButtonMenuModel() override = default;

 private:
  enum ContextMenuCommand { kHideAIChatButton };

  void Build() {
    AddItemWithStringId(ContextMenuCommand::kHideAIChatButton,
                        IDS_HIDE_LUXXLE_AI_CHAT_ICON_ON_TOOLBAR);
  }

  // ui::SimpleMenuModel::Delegate:
  void ExecuteCommand(int command_id, int event_flags) override {
    if (command_id == ContextMenuCommand::kHideAIChatButton) {
      prefs_->SetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton, false);
    }
  }

  raw_ref<PrefService> prefs_;
};

}  // namespace

AIChatButton::AIChatButton(Browser* browser)
    : ToolbarButton(base::BindRepeating(&AIChatButton::ButtonPressed,
                                        base::Unretained(this))),
      browser_(*browser) {
  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  SetMenuModel(std::make_unique<AIChatButtonMenuModel>(prefs));
  SetVectorIcon(kLeoProductLuxxleLeoIcon);

  // Visibility is managed by |ToolbarView|.
  SetVisible(false);

  SetTooltipText(l10n_util::GetStringUTF16(IDS_TOOLTIP_AI_CHAT_TOOLBAR_BUTTON));
  set_context_menu_controller(this);
  GetViewAccessibility().SetHasPopup(ax::mojom::HasPopup::kMenu);
}

AIChatButton::~AIChatButton() = default;

void AIChatButton::ButtonPressed() {
  // We could use settings for opening full page or panel.
  luxxle::ShowFullpageChat(base::to_address(browser_));

  auto* profile_metrics =
      misc_metrics::ProfileMiscMetricsServiceFactory::GetServiceForContext(
          browser_->profile());
  if (!profile_metrics) {
    return;
  }
  auto* ai_chat_metrics = profile_metrics->GetAIChatMetrics();
  if (!ai_chat_metrics) {
    return;
  }
  ai_chat_metrics->HandleOpenViaEntryPoint(ai_chat::EntryPoint::kToolbarButton);
}

BEGIN_METADATA(AIChatButton)
END_METADATA

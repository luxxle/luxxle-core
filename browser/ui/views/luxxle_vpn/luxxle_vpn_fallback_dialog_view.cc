/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_vpn/luxxle_vpn_fallback_dialog_view.h"

#include <memory>
#include <utility>

#include "luxxle/browser/luxxle_features_internal_names.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/about_flags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "components/constrained_window/constrained_window_views.h"
#include "components/grit/luxxle_components_strings.h"
#include "components/webui/flags/pref_service_flags_storage.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/mojom/dialog_button.mojom.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/link.h"
#include "ui/views/controls/styled_label.h"
#include "ui/views/layout/box_layout.h"

namespace luxxle {
void ShowLuxxleVpnIKEv2FallbackDialog() {
  luxxle_vpn::LuxxleVpnFallbackDialogView::Show(chrome::FindLastActive());
}
}  // namespace luxxle

namespace luxxle_vpn {

namespace {

constexpr int kChildSpacing = 16;
constexpr int kPadding = 24;
constexpr int kTopPadding = 32;
constexpr int kBottomPadding = 26;
constexpr int kDialogWidth = 600;

}  // namespace

// static
void LuxxleVpnFallbackDialogView::Show(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  if (!prefs->GetBoolean(prefs::kLuxxleVPNWireguardFallbackDialog)) {
    return;
  }

  constrained_window::CreateBrowserModalDialogViews(
      new LuxxleVpnFallbackDialogView(browser),
      browser->window()->GetNativeWindow())
      ->Show();
}

LuxxleVpnFallbackDialogView::LuxxleVpnFallbackDialogView(Browser* browser)
    : browser_(browser), prefs_(browser->profile()->GetPrefs()) {
  SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kVertical,
      gfx::Insets::TLBR(kTopPadding, kPadding, kBottomPadding, kPadding),
      kChildSpacing));
  SetButtons(static_cast<int>(ui::mojom::DialogButton::kOk) |
             static_cast<int>(ui::mojom::DialogButton::kCancel));
  SetButtonLabel(
      ui::mojom::DialogButton::kOk,
      l10n_util::GetStringUTF16(IDS_LUXXLE_VPN_FALLBACK_DIALOG_OK_TEXT));
  SetButtonLabel(
      ui::mojom::DialogButton::kCancel,
      l10n_util::GetStringUTF16(IDS_LUXXLE_VPN_FALLBACK_DIALOG_CANCEL_TEXT));
  SetAcceptCallback(base::BindOnce(&LuxxleVpnFallbackDialogView::OnAccept,
                                   base::Unretained(this)));
  auto* header_label = AddChildView(std::make_unique<views::Label>(
      l10n_util::GetStringUTF16(IDS_LUXXLE_VPN_FALLBACK_DIALOG_TITLE)));
  header_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  const std::u16string contents_text =
      l10n_util::GetStringUTF16(IDS_LUXXLE_VPN_FALLBACK_DIALOG_TEXT);

  auto* contents_label = AddChildView(std::make_unique<views::StyledLabel>());
  contents_label->SetTextContext(views::style::CONTEXT_DIALOG_BODY_TEXT);
  contents_label->SetText(contents_text);
  contents_label->SizeToFit(kDialogWidth);

  RegisterWindowClosingCallback(base::BindOnce(
      &LuxxleVpnFallbackDialogView::OnClosing, base::Unretained(this)));

  dont_ask_again_checkbox_ =
      AddChildView(std::make_unique<views::Checkbox>(l10n_util::GetStringUTF16(
          IDS_LUXXLE_VPN_DNS_SETTINGS_NOTIFICATION_DIALOG_CHECKBOX_TEXT)));
}

LuxxleVpnFallbackDialogView::~LuxxleVpnFallbackDialogView() = default;

ui::mojom::ModalType LuxxleVpnFallbackDialogView::GetModalType() const {
  return ui::mojom::ModalType::kWindow;
}

bool LuxxleVpnFallbackDialogView::ShouldShowCloseButton() const {
  return false;
}

bool LuxxleVpnFallbackDialogView::ShouldShowWindowTitle() const {
  return false;
}

void LuxxleVpnFallbackDialogView::OnClosing() {
  prefs_->SetBoolean(prefs::kLuxxleVPNWireguardFallbackDialog,
                     !dont_ask_again_checkbox_->GetChecked());
}

void LuxxleVpnFallbackDialogView::OnAccept() {
  g_browser_process->local_state()->SetBoolean(prefs::kLuxxleVPNWireguardEnabled,
                                               false);
}

BEGIN_METADATA(LuxxleVpnFallbackDialogView)
END_METADATA

}  // namespace luxxle_vpn

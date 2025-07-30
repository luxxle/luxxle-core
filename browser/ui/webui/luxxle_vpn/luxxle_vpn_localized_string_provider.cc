/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_vpn/luxxle_vpn_localized_string_provider.h"

#include "base/no_destructor.h"
#include "components/grit/luxxle_components_strings.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/webui/web_ui_util.h"

namespace luxxle_vpn {

void AddLocalizedStrings(content::WebUIDataSource* html_source) {
  static constexpr webui::LocalizedString kLocalizedStrings[] = {
      {"luxxleVpn", IDS_LUXXLE_VPN},
      {"luxxleVpnConnect", IDS_LUXXLE_VPN_CONNECT},
      {"luxxleVpnConnecting", IDS_LUXXLE_VPN_CONNECTING},
      {"luxxleVpnConnected", IDS_LUXXLE_VPN_CONNECTED},
      {"luxxleVpnDisconnecting", IDS_LUXXLE_VPN_DISCONNECTING},
      {"luxxleVpnDisconnected", IDS_LUXXLE_VPN_DISCONNECTED},
      {"luxxleVpnConnectionFailed", IDS_LUXXLE_VPN_CONNECTION_FAILED},
      {"luxxleVpnUnableConnectToServer", IDS_LUXXLE_VPN_UNABLE_CONNECT_TO_SERVER},
      {"luxxleVpnTryAgain", IDS_LUXXLE_VPN_TRY_AGAIN},
      {"luxxleVpnChooseAnotherServer", IDS_LUXXLE_VPN_CHOOSE_ANOTHER_SERVER},
      {"luxxleVpnUnableConnectInfo", IDS_LUXXLE_VPN_UNABLE_CONNECT_INFO},
      {"luxxleVpnBuy", IDS_LUXXLE_VPN_BUY},
      {"luxxleVpnPurchased", IDS_LUXXLE_VPN_HAS_PURCHASED},
      {"luxxleVpnPoweredBy", IDS_LUXXLE_VPN_POWERED_BY},
      {"luxxleVpnSettingsPanelHeader", IDS_LUXXLE_VPN_SETTINGS_PANEL_HEADER},
      {"luxxleVpnSettingsPanelBackButtonAriaLabel",
       IDS_LUXXLE_VPN_SETTINGS_PANEL_BACK_BUTTON_ARIA_LABEL},
      {"luxxleVpnErrorPanelBackButtonAriaLabel",
       IDS_LUXXLE_VPN_PANEL_GO_TO_MAIN_BUTTON_ARIA_LABEL},
      {"luxxleVpnStatus", IDS_LUXXLE_VPN_STATUS},
      {"luxxleVpnExpires", IDS_LUXXLE_VPN_EXPIRES},
      {"luxxleVpnManageSubscription", IDS_LUXXLE_VPN_MANAGE_SUBSCRIPTION},
      {"luxxleVpnReconnectAutomatically", IDS_LUXXLE_VPN_RECONNECT_AUTOMATICALLY},
      {"luxxleVpnContactSupport", IDS_LUXXLE_VPN_CONTACT_SUPPORT},
      {"luxxleVpnAbout", IDS_LUXXLE_VPN_ABOUT},
      {"luxxleVpnFeature1", IDS_LUXXLE_VPN_FEATURE_1},
      {"luxxleVpnFeature2", IDS_LUXXLE_VPN_FEATURE_2},
      {"luxxleVpnFeature3", IDS_LUXXLE_VPN_FEATURE_3},
      {"luxxleVpnFeature4", IDS_LUXXLE_VPN_FEATURE_4},
      {"luxxleVpnFeature5", IDS_LUXXLE_VPN_FEATURE_5},
      {"luxxleVpnLoading", IDS_LUXXLE_VPN_LOADING},
      {"luxxleVpnPurchaseFailed", IDS_LUXXLE_VPN_PURCHASE_FAILED},
      {"luxxleVpnSelectYourServer", IDS_LUXXLE_VPN_SELECT_YOUR_SERVER},
      {"luxxleVpnServerSelectionSingleCity",
       IDS_LUXXLE_VPN_SERVER_SELECTION_SINGLE_CITY},
      {"luxxleVpnServerSelectionMultipleCities",
       IDS_LUXXLE_VPN_SERVER_SELECTION_MULTIPLE_CITIES},
      {"luxxleVpnServerSelectionSingleServer",
       IDS_LUXXLE_VPN_SERVER_SELECTION_SINGLE_SERVER},
      {"luxxleVpnServerSelectionMultipleServers",
       IDS_LUXXLE_VPN_SERVER_SELECTION_MULTIPLE_SERVERS},
      {"luxxleVpnServerSelectionAutomaticLabel",
       IDS_LUXXLE_VPN_SERVER_SELECTION_AUTOMATIC_LABEL},
      {"luxxleVpnServerSelectionOptimalLabel",
       IDS_LUXXLE_VPN_SERVER_SELECTION_OPTIMAL_LABEL},
      {"luxxleVpnServerSelectionOptimalDesc",
       IDS_LUXXLE_VPN_SERVER_SELECTION_OPTIMAL_DESC},
      {"luxxleVpnSelectPanelBackButtonAriaLabel",
       IDS_LUXXLE_VPN_PANEL_GO_TO_MAIN_BUTTON_ARIA_LABEL},
      {"luxxleVpnSupportTicketFailed", IDS_LUXXLE_VPN_SUPPORT_TICKET_FAILED},
      {"luxxleVpnEditPaymentMethod", IDS_LUXXLE_VPN_EDIT_PAYMENT},
      {"luxxleVpnPaymentFailure", IDS_LUXXLE_VPN_PAYMENT_FAILURE},
      {"luxxleVpnPaymentFailureReason", IDS_LUXXLE_VPN_PAYMENT_FAILURE_REASON},
      {"luxxleVpnSupportPanelBackButtonAriaLabel",
       IDS_LUXXLE_VPN_SUPPORT_PANEL_BACK_BUTTON_ARIA_LABEL},
      {"luxxleVpnSupportEmail", IDS_LUXXLE_VPN_SUPPORT_EMAIL},
      {"luxxleVpnSupportEmailInputPlaceholder",
       IDS_LUXXLE_VPN_SUPPORT_EMAIL_PLACEHOLDER},
      {"luxxleVpnSupportEmailNotValid", IDS_LUXXLE_VPN_SUPPORT_EMAIL_NOT_VALID},
      {"luxxleVpnSupportFieldIsRequired",
       IDS_LUXXLE_VPN_SUPPORT_FIELD_IS_REQUIRED},
      {"luxxleVpnSupportSubject", IDS_LUXXLE_VPN_SUPPORT_SUBJECT},
      {"luxxleVpnSupportSubjectNotSet", IDS_LUXXLE_VPN_SUPPORT_SUBJECT_NOTSET},
      {"luxxleVpnSupportSubjectOtherConnectionProblem",
       IDS_LUXXLE_VPN_SUPPORT_SUBJECT_OTHER_CONNECTION_PROBLEM},
      {"luxxleVpnSupportSubjectNoInternet",
       IDS_LUXXLE_VPN_SUPPORT_SUBJECT_NO_INTERNET},
      {"luxxleVpnSupportSubjectSlowConnection",
       IDS_LUXXLE_VPN_SUPPORT_SUBJECT_SLOW_CONNECTION},
      {"luxxleVpnSupportSubjectWebsiteDoesntWork",
       IDS_LUXXLE_VPN_SUPPORT_SUBJECT_WEBSITE_DOESNT_WORK},
      {"luxxleVpnSupportSubjectOther", IDS_LUXXLE_VPN_SUPPORT_SUBJECT_OTHER},
      {"luxxleVpnSupportBody", IDS_LUXXLE_VPN_SUPPORT_BODY},
      {"luxxleVpnSupportDescriptionPlaceholder",
       IDS_LUXXLE_VPN_SUPPORT_BODY_PLACEHOLDER},
      {"luxxleVpnSupportOptionalHeader", IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_HEADER},
      {"luxxleVpnSupportOptionalNotes", IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_NOTES},
      {"luxxleVpnSupportOptionalNotesPrivacyPolicy",
       IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_NOTES_PRIVACY_POLICY},
      {"luxxleVpnSupportOptionalVpnHostname",
       IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_VPN_HOSTNAME},
      {"luxxleVpnSupportOptionalAppVersion",
       IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_APP_VERSION},
      {"luxxleVpnSupportOptionalOsVersion",
       IDS_LUXXLE_VPN_SUPPORT_OPTIONAL_OS_VERSION},
      {"luxxleVpnSupportNotes", IDS_LUXXLE_VPN_SUPPORT_NOTES},
      {"luxxleVpnSupportSubmit", IDS_LUXXLE_VPN_SUPPORT_SUBMIT},
      {"luxxleVpnConnectNotAllowed", IDS_LUXXLE_VPN_CONNECT_NOT_ALLOWED},
      {"luxxleVpnSupportTimezone", IDS_LUXXLE_VPN_SUPPORT_TIMEZONE},
      {"luxxleVpnSessionExpiredTitle",
       IDS_LUXXLE_VPN_MAIN_PANEL_SESSION_EXPIRED_PART_TITLE},
      {"luxxleVpnSettingsTooltip", IDS_LUXXLE_VPN_MAIN_PANEL_VPN_SETTINGS_TITLE},
      {"luxxleVpnSessionExpiredContent",
       IDS_LUXXLE_VPN_MAIN_PANEL_SESSION_EXPIRED_PART_CONTENT},
      {"luxxleVpnOutOfCredentials",
       IDS_LUXXLE_VPN_MAIN_PANEL_OUT_OF_CREDENTIALS_TITLE},
  };

  for (const auto& str : kLocalizedStrings) {
    std::u16string l10n_str = l10n_util::GetStringUTF16(str.id);
    html_source->AddString(str.name, l10n_str);
  }
}

}  // namespace luxxle_vpn

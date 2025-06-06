/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/ai_chat/core/browser/constants.h"

#include <array>
#include <functional>
#include <string>
#include <utility>

#include "base/containers/flat_tree.h"
#include "base/strings/string_util.h"
#include "components/grit/brave_components_strings.h"
#include "mojo/public/cpp/bindings/struct_ptr.h"
#include "ui/base/l10n/l10n_util.h"

namespace ai_chat {

base::span<const webui::LocalizedString> GetLocalizedStrings() {
  static constexpr auto kLocalizedStrings = std::to_array<
      webui::LocalizedString>(
      {{"siteTitle", IDS_CHAT_UI_TITLE},
       {"pwaTitle", IDS_CHAT_UI_PWA_TITLE},
       {"summarizeFailedLabel", IDS_CHAT_UI_SUMMARIZE_FAILED_LABEL},
       {"acceptButtonLabel", IDS_CHAT_UI_ACCEPT_BUTTON_LABEL},
       {"summarizeButtonLabel", IDS_CHAT_UI_SUMMARIZE_BUTTON_LABEL},
       {"aboutTitle", IDS_CHAT_UI_ABOUT_TITLE},
       {"aboutDescription", IDS_CHAT_UI_ABOUT_DESCRIPTION},
       {"aboutDescription_2", IDS_CHAT_UI_ABOUT_DESCRIPTION_2},
       {"aboutDescription_3", IDS_CHAT_UI_ABOUT_DESCRIPTION_3},
       {"initialPlaceholderLabel", IDS_CHAT_UI_INITIAL_PLACEHOLDER_LABEL},
       {"placeholderLabel", IDS_CHAT_UI_PLACEHOLDER_LABEL},
       {"pageContentWarning", IDS_CHAT_UI_PAGE_CONTENT_WARNING},
       {"customModelInvalidEndpoint", IDS_CUSTOM_MODEL_ENDPOINT_INVALID_ERROR},
       {"customModelModifyConfigurationLabel",
        IDS_CHAT_UI_MODIFY_CONFIGURATION_LABEL},
       {"errorNetworkLabel", IDS_CHAT_UI_ERROR_NETWORK},
       {"errorRateLimit", IDS_CHAT_UI_ERROR_RATE_LIMIT},
       {"errorInvalidAPIKey", IDS_CHAT_UI_ERROR_INVALID_API_KEY},
       {"errorOAIRateLimit", IDS_CHAT_UI_ERROR_OAI_RATE_LIMIT},
       {"errorServiceOverloaded", IDS_CHAT_UI_ERROR_SERVICE_OVERLOADED},
       {"retryButtonLabel", IDS_CHAT_UI_RETRY_BUTTON_LABEL},
       {"introMessage-chat-basic", IDS_CHAT_UI_INTRO_MESSAGE_CHAT_BASIC},
       {"introMessage-chat-qwen", IDS_CHAT_UI_INTRO_MESSAGE_CHAT_QWEN},
       {"introMessage-chat-deepseek-r1",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_DEEPSEEK_R1},
       {"introMessage-chat-claude-instant",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_LEO_CLAUDE_INSTANT},
       {"introMessage-chat-claude-haiku",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_LEO_CLAUDE_HAIKU},
       {"introMessage-chat-claude-sonnet",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_LEO_CLAUDE_SONNET},
       {"introMessage-chat-vision-basic",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_VISION_BASIC},
       {"introMessage-chat-automatic",
        IDS_CHAT_UI_INTRO_MESSAGE_CHAT_AUTOMATIC},
       {"modelPremiumLabelNonPremium",
        IDS_CHAT_UI_MODEL_PREMIUM_LABEL_NON_PREMIUM},
       {"modelCategory-chat", IDS_CHAT_UI_MODEL_CATEGORY_CHAT},
       {"menuNewChat", IDS_CHAT_UI_MENU_NEW_CHAT},
       {"openFullPageLabel", IDS_CHAT_UI_LABEL_OPEN_FULL_PAGE},
       {"closeLabel", IDS_CHAT_UI_LABEL_CLOSE},
       {"menuGoPremium", IDS_CHAT_UI_MENU_GO_PREMIUM},
       {"menuManageSubscription", IDS_CHAT_UI_MENU_MANAGE_SUBSCRIPTION},
       {"menuConversationHistory", IDS_CHAT_UI_MENU_CONVERSATION_HISTORY},
       {"menuSettings", IDS_CHAT_UI_MENU_SETTINGS},
       {"menuTitleModels", IDS_CHAT_UI_MENU_TITLE_MODELS},
       {"suggestQuestionsLabel", IDS_CHAT_UI_SUGGEST_QUESTIONS_LABEL},
       {"upgradeButtonLabel", IDS_CHAT_UI_UPGRADE_BUTTON_LABEL},
       {"rateLimitReachedTitle", IDS_CHAT_UI_RATE_LIMIT_REACHED_TITLE},
       {"rateLimitReachedDesc", IDS_CHAT_UI_RATE_LIMIT_REACHED_DESC},
       {"premiumFeature_1", IDS_CHAT_UI_PREMIUM_FEATURE_1},
       {"premiumFeature_2", IDS_CHAT_UI_PREMIUM_FEATURE_2},
       {"premiumFeature_3", IDS_CHAT_UI_PREMIUM_FEATURE_3},
       {"premiumFeature_4", IDS_CHAT_UI_PREMIUM_FEATURE_4},
       {"premiumFeature_1_desc", IDS_CHAT_UI_PREMIUM_FEATURE_1_DESC},
       {"premiumFeature_2_desc", IDS_CHAT_UI_PREMIUM_FEATURE_2_DESC},
       {"premiumFeature_3_desc", IDS_CHAT_UI_PREMIUM_FEATURE_3_DESC},
       {"premiumFeature_4_desc", IDS_CHAT_UI_PREMIUM_FEATURE_4_DESC},
       {"premiumLabel", IDS_CHAT_UI_PREMIUM_LABEL},
       {"premiumPricing", IDS_CHAT_UI_PREMIUM_PRICING},
       {"premiumAnnualPricing", IDS_CHAT_UI_PREMIUM_ANNUAL_PRICING},
       {"monthlyLabel", IDS_CHAT_UI_MONTHLY_LABEL},
       {"oneYearLabel", IDS_CHAT_UI_ONE_YEAR_LABEL},
       {"bestValueLabel", IDS_CHAT_UI_BEST_VALUE_LABEL},
       {"switchToBasicModelButtonLabel",
        IDS_CHAT_UI_SWITCH_TO_BASIC_MODEL_BUTTON_LABEL},
       {"dismissButtonLabel", IDS_CHAT_UI_DISMISS_BUTTON_LABEL},
       {"unlockPremiumTitle", IDS_CHAT_UI_UNLOCK_PREMIUM_TITLE},
       {"premiumRefreshWarningDescription",
        IDS_CHAT_UI_PREMIUM_REFRESH_WARNING_DESCRIPTION},
       {"premiumRefreshWarningAction",
        IDS_CHAT_UI_PREMIUM_REFRESH_WARNING_ACTION},
       {"feedbackSent", IDS_CHAT_UI_FEEDBACK_SENT},
       {"answerDisliked", IDS_CHAT_UI_ANSWER_DISLIKED},
       {"answerLiked", IDS_CHAT_UI_ANSWER_LIKED},
       {"addFeedbackButtonLabel", IDS_CHAT_UI_ADD_FEEDBACK_BUTTON_LABEL},
       {"copyButtonLabel", IDS_CHAT_UI_COPY_BUTTON_LABEL},
       {"likeAnswerButtonLabel", IDS_CHAT_UI_LIKE_ANSWER_BUTTON_LABEL},
       {"dislikeAnswerButtonLabel", IDS_CHAT_UI_DISLIKE_ANSWER_BUTTON_LABEL},
       {"provideFeedbackTitle", IDS_CHAT_UI_PROVIDE_FEEDBACK_TITLE},
       {"selectFeedbackTopic", IDS_CHAT_UI_SELECT_FEEDBACK_TOPIC},
       {"feedbackCategoryLabel", IDS_CHAT_UI_FEEDBACK_CATEGORY_LABEL},
       {"feedbackDescriptionLabel", IDS_CHAT_UI_FEEDBACK_DESCRIPTION_LABEL},
       {"feedbackPremiumNote", IDS_CHAT_UI_FEEDBACK_PREMIUM_NOTE},
       {"submitButtonLabel", IDS_CHAT_UI_SUBMIT_BUTTON_LABEL},
       {"cancelButtonLabel", IDS_CHAT_UI_CANCEL_BUTTON_LABEL},
       {"deleteButtonLabel", IDS_CHAT_UI_DELETE_BUTTON_LABEL},
       {"deleteConversationWarning", IDS_CHAT_UI_DELETE_CONVERSATION_WARNING},
       {"saveButtonLabel", IDS_CHAT_UI_SAVE_BUTTON_LABEL},
       {"editedLabel", IDS_CHAT_UI_EDITED_LABEL},
       {"editButtonLabel", IDS_CHAT_UI_EDIT_BUTTON_LABEL},
       {"regenerateAnswerButtonLabel",
        IDS_CHAT_UI_REGENERATE_ANSWER_BUTTON_LABEL},
       {"regenerateAnswerMenuTitle", IDS_CHAT_UI_REGENERATE_ANSWER_MENU_TITLE},
       {"editPromptButtonLabel", IDS_CHAT_UI_EDIT_PROMPT_BUTTON_LABEL},
       {"copyPromptButtonLabel", IDS_CHAT_UI_COPY_PROMPT_BUTTON_LABEL},
       {"editAnswerLabel", IDS_CHAT_UI_EDIT_ANSWER_LABEL},
       {"optionNotHelpful", IDS_CHAT_UI_OPTION_NOT_HELPFUL},
       {"optionIncorrect", IDS_CHAT_UI_OPTION_INCORRECT},
       {"optionUnsafeHarmful", IDS_CHAT_UI_OPTION_UNSAFE_HARMFUL},
       {"optionOther", IDS_CHAT_UI_OPTION_OTHER},
       {"feedbackError", IDS_CHAT_UI_FEEDBACK_SUBMIT_ERROR},
       {"ratingError", IDS_CHAT_UI_RATING_ERROR},
       {"braveLeoModelSubtitle-chat-basic", IDS_CHAT_UI_CHAT_BASIC_SUBTITLE},
       {"braveLeoModelSubtitle-chat-claude-instant",
        IDS_CHAT_UI_CHAT_CLAUDE_INSTANT_SUBTITLE},
       {"braveLeoModelSubtitle-chat-claude-haiku",
        IDS_CHAT_UI_CHAT_CLAUDE_HAIKU_SUBTITLE},
       {"braveLeoModelSubtitle-chat-claude-sonnet",
        IDS_CHAT_UI_CHAT_CLAUDE_SONNET_SUBTITLE},
       {"braveLeoModelSubtitle-chat-qwen", IDS_CHAT_UI_CHAT_QWEN_SUBTITLE},
       {"braveLeoModelSubtitle-chat-deepseek-r1",
        IDS_CHAT_UI_CHAT_DEEPSEEK_R1_SUBTITLE},
       {"braveLeoModelSubtitle-chat-vision-basic",
        IDS_CHAT_UI_CHAT_VISION_BASIC_SUBTITLE},
       {"braveLeoModelSubtitle-chat-automatic",
        IDS_CHAT_UI_CHAT_AUTOMATIC_SUBTITLE},
       {"clearChatButtonLabel", IDS_CHAT_UI_CLEAR_CHAT_BUTTON_LABEL},
       {"newChatButtonLabel", IDS_CHAT_UI_NEW_CONVERSATION_BUTTON_LABEL},
       {"menuRenameConversation", IDS_CHAT_UI_MENU_RENAME_CONVERSATION},
       {"menuDeleteConversation", IDS_CHAT_UI_MENU_DELETE_CONVERSATION},
       {"reasoningLabel", IDS_CHAT_UI_REASONING_LABEL},
       {"reasoningCompleteLabel", IDS_CHAT_UI_REASONING_COMPLETE_LABEL},
       {"hideDetailsButtonLabel", IDS_CHAT_UI_HIDE_DETAILS_BUTTON_LABEL},
       {"showDetailsButtonLabel", IDS_CHAT_UI_SHOW_DETAILS_BUTTON_LABEL},
       {"openLabel", IDS_CHAT_UI_OPEN_LABEL},
       {"openExternalLink", IDS_CHAT_UI_OPEN_EXTERNAL_LINK},
       {"openExternalLinkInfo", IDS_CHAT_UI_OPEN_EXTERNAL_LINK_INFO},
       {"openExternalLinkCheckboxLabel",
        IDS_CHAT_UI_OPEN_EXTERNAL_LINK_CHECKBOX_LABEL},
       {"sendButtonLabel", IDS_CHAT_UI_SEND_BUTTON_LABEL},
       {"rateMessagePrivacyModalTitle",
        IDS_CHAT_UI_RATE_MESSAGE_PRIVACY_MODAL_TITLE},
       {"rateMessagePrivacyModalDescription",
        IDS_CHAT_UI_RATE_MESSAGE_PRIVACY_MODAL_DESCRIPTION},
       {"sendChatButtonLabel", IDS_CHAT_UI_SEND_CHAT_BUTTON_LABEL},
       {"stopGenerationButtonLabel", IDS_CHAT_UI_STOP_GENERATION_BUTTON_LABEL},
       {"errorContextLimitReaching", IDS_CHAT_UI_ERROR_CONTEXT_LIMIT_REACHING},
       {"gotItButtonLabel", IDS_CHAT_UI_GOT_IT_BUTTON_LABEL},
       {"pageContentTooLongWarning", IDS_CHAT_UI_PAGE_CONTENT_TOO_LONG_WARNING},
       {"trimmedTokensWarning", IDS_CHAT_UI_TRIMMED_TOKENS_WARNING},
       {"errorConversationEnd", IDS_CHAT_UI_CONVERSATION_END_ERROR},
       {"searchInProgress", IDS_CHAT_UI_SEARCH_IN_PROGRESS},
       {"searchQueries", IDS_CHAT_UI_SEARCH_QUERIES},
       {"sources", IDS_CHAT_UI_SOURCES},
       {"expandSources", IDS_CHAT_UI_EXPAND_SOURCES},
       {"learnMore", IDS_CHAT_UI_LEARN_MORE},
       {"closeNotice", IDS_CHAT_UI_CLOSE_NOTICE},
       {"searchTabsPlaceholder", IDS_CHAT_UI_ATTACHMENTS_SEARCH_PLACEHOLDER},
       {"attachmentsTitle", IDS_CHAT_UI_ATTACHMENTS_TITLE},
       {"attachmentsDescription", IDS_CHAT_UI_ATTACHMENTS_DESCRIPTION},
       {"attachmentsBrowserTabsTitle",
        IDS_CHAT_UI_ATTACHMENTS_BROWSER_TABS_TITLE},
       {"noticeConversationHistoryBody",
        IDS_CHAT_UI_NOTICE_CONVERSATION_HISTORY_BODY},
       {"noticeConversationHistoryEmpty",
        IDS_CHAT_UI_NOTICE_CONVERSATION_HISTORY_EMPTY},
       {"noticeConversationHistoryTitleDisabledPref",
        IDS_CHAT_UI_NOTICE_CONVERSATION_HISTORY_TITLE_DISABLED_PREF},
       {"noticeConversationHistoryDisabledPref",
        IDS_CHAT_UI_NOTICE_CONVERSATION_HISTORY_DISABLED_PREF},
       {"noticeConversationHistoryDisabledPrefButton",
        IDS_CHAT_UI_NOTICE_CONVERSATION_HISTORY_DISABLED_PREF_BUTTON},
       {"leoSettingsTooltipLabel", IDS_CHAT_UI_LEO_SETTINGS_TOOLTIP_LABEL},
       {"summarizePageButtonLabel", IDS_CHAT_UI_SUMMARIZE_PAGE},
       {"welcomeGuideTitle", IDS_CHAT_UI_WELCOME_GUIDE_TITLE},
       {"welcomeGuideSubtitle", IDS_CHAT_UI_WELCOME_GUIDE_SUBTITLE},
       {"welcomeGuideSiteHelpCardTitle",
        IDS_CHAT_UI_WELCOME_GUIDE_SITE_HELP_CARD_TITLE},
       {"welcomeGuideSiteHelpCardDesc",
        IDS_CHAT_UI_WELCOME_GUIDE_SITE_HELP_CARD_DESC},
       {"welcomeGuideSiteHelpCardDescWithAction",
        IDS_CHAT_UI_WELCOME_GUIDE_SITE_HELP_CARD_WITH_ACTION},
       {"welcomeGuideShatCardTitle", IDS_CHAT_UI_WELCOME_GUIDE_CHAT_CARD_TITLE},
       {"welcomeGuideShatCardDesc", IDS_CHAT_UI_WELCOME_GUIDE_CHAT_CARD_DESC},
       {"privacyTitle", IDS_CHAT_UI_PRIVACY_TITLE},
       {"contextToggleLabel", IDS_CHAT_UI_CONTEXT_TOGGLE_LABEL},
       {"contextToggleTooltipInfo", IDS_CHAT_UI_CONTEXT_TOGGLE_TOOLTIP_INFO},
       {"subscriptionPolicyInfo", IDS_CHAT_UI_SUBSCRIPTION_POLICY_INFO},
       {"explainLabel", IDS_AI_CHAT_CONTEXT_EXPLAIN},
       {"summarizeLabel", IDS_AI_CHAT_CONTEXT_SUMMARIZE_TEXT},
       {"paraphraseLabel", IDS_AI_CHAT_CONTEXT_PARAPHRASE},
       {"createCategoryTitle", IDS_AI_CHAT_CONTEXT_CREATE},
       {"taglineLabel", IDS_AI_CHAT_CONTEXT_CREATE_TAGLINE},
       {"socialMediaPostLabel", IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_POST},
       {"socialMediaShortLabel",
        IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_COMMENT_SHORT},
       {"socialMediaLongLabel",
        IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_COMMENT_LONG},
       {"rewriteCategoryTitle", IDS_AI_CHAT_CONTEXT_REWRITE},
       {"improveLabel", IDS_AI_CHAT_CONTEXT_IMPROVE},
       {"changeToneLabel", IDS_AI_CHAT_CONTEXT_CHANGE_TONE},
       {"changeLengthLabel", IDS_AI_CHAT_CONTEXT_CHANGE_LENGTH},
       {"academicizeLabel", IDS_AI_CHAT_CONTEXT_ACADEMICIZE},
       {"professionalizeLabel", IDS_AI_CHAT_CONTEXT_PROFESSIONALIZE},
       {"persuasiveToneLabel", IDS_AI_CHAT_CONTEXT_PERSUASIVE_TONE},
       {"casualizeLabel", IDS_AI_CHAT_CONTEXT_CASUALIZE},
       {"funnyToneLabel", IDS_AI_CHAT_CONTEXT_FUNNY_TONE},
       {"shortenLabel", IDS_AI_CHAT_CONTEXT_SHORTEN},
       {"expandLabel", IDS_AI_CHAT_CONTEXT_EXPAND},
       {"sendSiteHostnameLabel", IDS_CHAT_UI_SEND_SITE_HOSTNAME_LABEL},
       {"maybeLaterLabel", IDS_AI_CHAT_MAYBE_LATER_LABEL},
       {"toolsMenuButtonLabel", IDS_AI_CHAT_LEO_TOOLS_BUTTON_LABEL},
       {"attachmentMenuButtonLabel",
        IDS_AI_CHAT_LEO_ATTACHMENT_MENU_BUTTON_LABEL},
       {"uploadFileButtonLabel", IDS_AI_CHAT_UPLOAD_FILE_BUTTON_LABEL},
       {"uploadingFileLabel", IDS_AI_CHAT_UPLOADING_FILE_LABEL},
       {"takeAPictureButtonLabel", IDS_AI_CHAT_TAKE_A_PICTURE_BUTTON_LABEL},
       {"screenshotButtonLabel", IDS_AI_CHAT_SCREENSHOT_BUTTON_LABEL},
       {"currentTabContentsButtonLabel",
        IDS_AI_CHAT_CURRENT_TAB_CONTENTS_BUTTON_LABEL},
       {"useMicButtonLabel", IDS_AI_CHAT_USE_MICROPHONE_BUTTON_LABEL},
       {"menuTitleCustomModels", IDS_AI_CHAT_MENU_TITLE_CUSTOM_MODELS},
       {"startConversationLabel", IDS_AI_CHAT_START_CONVERSATION_LABEL},
       {"goBackToActiveConversationButton",
        IDS_AI_CHAT_GO_BACK_TO_ACTIVE_CONVERSATION_BUTTON},
       {"conversationListUntitled", IDS_AI_CHAT_CONVERSATION_LIST_UNTITLED},
       {"temporaryChatLabel", IDS_AI_CHAT_TEMPORARY_CHAT_LABEL},
       {"temporaryChatInfo", IDS_AI_CHAT_TEMPORARY_CHAT_DESCRIPTION}});

  return kLocalizedStrings;
}

std::vector<mojom::ActionGroupPtr> GetActionMenuList() {
  std::vector<mojom::ActionGroupPtr> action_list;

  {
    std::vector<mojom::ActionEntryPtr> entries;
    mojom::ActionGroupPtr group = mojom::ActionGroup::New(
        l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_QUICK_ACTIONS),
        std::move(entries));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_EXPLAIN),
            mojom::ActionType::EXPLAIN)));

    action_list.push_back(std::move(group));
  }

  {
    std::vector<mojom::ActionEntryPtr> entries;
    mojom::ActionGroupPtr group = mojom::ActionGroup::New(
        l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_REWRITE),
        std::move(entries));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_PARAPHRASE),
            mojom::ActionType::PARAPHRASE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_IMPROVE),
            mojom::ActionType::IMPROVE)));

    // Subheading
    auto change_tone_subheading =
        l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_CHANGE_TONE);
    group->entries.push_back(
        mojom::ActionEntry::NewSubheading(change_tone_subheading));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_ACADEMICIZE)},
                " / "),
            mojom::ActionType::ACADEMICIZE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_PROFESSIONALIZE)},
                " / "),
            mojom::ActionType::PROFESSIONALIZE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_PERSUASIVE_TONE)},
                " / "),
            mojom::ActionType::PERSUASIVE_TONE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_CASUALIZE)},
                " / "),
            mojom::ActionType::CASUALIZE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_FUNNY_TONE)},
                " / "),
            mojom::ActionType::FUNNY_TONE)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_SHORTEN)},
                " / "),
            mojom::ActionType::SHORTEN)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {change_tone_subheading,
                 l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_EXPAND)},
                " / "),
            mojom::ActionType::EXPAND)));

    action_list.push_back(std::move(group));
  }

  {
    std::vector<mojom::ActionEntryPtr> entries;
    mojom::ActionGroupPtr group = mojom::ActionGroup::New(
        l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_CREATE),
        std::move(entries));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_CREATE_TAGLINE),
            mojom::ActionType::CREATE_TAGLINE)));

    // Subheading
    auto social_media_subheading =
        l10n_util::GetStringUTF8(IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_POST);
    group->entries.push_back(
        mojom::ActionEntry::NewSubheading(social_media_subheading));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {social_media_subheading,
                 l10n_util::GetStringUTF8(
                     IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_COMMENT_SHORT)},
                " / "),
            mojom::ActionType::CREATE_SOCIAL_MEDIA_COMMENT_SHORT)));

    group->entries.push_back(
        mojom::ActionEntry::NewDetails(mojom::ActionDetails::New(
            base::JoinString(
                {social_media_subheading,
                 l10n_util::GetStringUTF8(
                     IDS_AI_CHAT_CONTEXT_CREATE_SOCIAL_MEDIA_COMMENT_LONG)},
                " / "),
            mojom::ActionType::CREATE_SOCIAL_MEDIA_COMMENT_LONG)));

    action_list.push_back(std::move(group));
  }

  return action_list;
}
}  // namespace ai_chat

// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/api/omnibox/autocomplete_classifier.h"

#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "components/omnibox/browser/autocomplete_classifier.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/autocomplete/model/autocomplete_classifier_factory.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/prefs/pref_names.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "net/base/apple/url_conversions.h"

namespace luxxle {
AutocompleteMatch::Type MatchTypeFromLuxxleType(
    LuxxleIOSAutocompleteMatchType type) {
  switch (type) {
    case LuxxleIOSAutocompleteMatchTypeUrlWhatYouTyped:
      return AutocompleteMatch::Type::URL_WHAT_YOU_TYPED;
    case LuxxleIOSAutocompleteMatchTypeHistoryUrl:
      return AutocompleteMatch::Type::HISTORY_URL;
    case LuxxleIOSAutocompleteMatchTypeNavSuggest:
      return AutocompleteMatch::Type::NAVSUGGEST;
    case LuxxleIOSAutocompleteMatchTypeSearchWhatYouTyped:
      return AutocompleteMatch::Type::SEARCH_WHAT_YOU_TYPED;
    case LuxxleIOSAutocompleteMatchTypeSearchHistory:
      return AutocompleteMatch::Type::SEARCH_HISTORY;
    case LuxxleIOSAutocompleteMatchTypeSearchOtherEngine:
      return AutocompleteMatch::Type::SEARCH_OTHER_ENGINE;
    case LuxxleIOSAutocompleteMatchTypeBookmarkTitle:
      return AutocompleteMatch::Type::BOOKMARK_TITLE;
    case LuxxleIOSAutocompleteMatchTypeClipboardUrl:
      return AutocompleteMatch::Type::CLIPBOARD_URL;
    case LuxxleIOSAutocompleteMatchTypeClipboardText:
      return AutocompleteMatch::Type::CLIPBOARD_TEXT;
    case LuxxleIOSAutocompleteMatchTypeOpenTab:
      return AutocompleteMatch::Type::OPEN_TAB;
  }
  NOTREACHED() << "Unknown LuxxleIOSAutocompleteMatchType: " << type;
}

LuxxleIOSAutocompleteMatchType LuxxleTypeFromMatchType(
    AutocompleteMatch::Type type) {
  switch (type) {
    case AutocompleteMatch::Type::URL_WHAT_YOU_TYPED:
      return LuxxleIOSAutocompleteMatchTypeUrlWhatYouTyped;
    case AutocompleteMatch::Type::HISTORY_URL:
      return LuxxleIOSAutocompleteMatchTypeHistoryUrl;
    case AutocompleteMatch::Type::NAVSUGGEST:
      return LuxxleIOSAutocompleteMatchTypeNavSuggest;
    case AutocompleteMatch::Type::SEARCH_WHAT_YOU_TYPED:
      return LuxxleIOSAutocompleteMatchTypeSearchWhatYouTyped;
    case AutocompleteMatch::Type::SEARCH_HISTORY:
      return LuxxleIOSAutocompleteMatchTypeSearchHistory;
    case AutocompleteMatch::Type::SEARCH_OTHER_ENGINE:
      return LuxxleIOSAutocompleteMatchTypeSearchOtherEngine;
    case AutocompleteMatch::Type::BOOKMARK_TITLE:
      return LuxxleIOSAutocompleteMatchTypeBookmarkTitle;
    case AutocompleteMatch::Type::CLIPBOARD_URL:
      return LuxxleIOSAutocompleteMatchTypeClipboardUrl;
    case AutocompleteMatch::Type::CLIPBOARD_TEXT:
      return LuxxleIOSAutocompleteMatchTypeClipboardText;
    case AutocompleteMatch::Type::OPEN_TAB:
      return LuxxleIOSAutocompleteMatchTypeOpenTab;
    default:
      NOTREACHED() << "Unknown AutocompleteMatch::Type: " << type;
  }
}
}  // namespace luxxle

@implementation LuxxleIOSAutocompleteMatch
- (instancetype)initWithText:(NSString*)text
                        type:(LuxxleIOSAutocompleteMatchType)type
              destinationURL:(NSURL*)destinationURL {
  if ((self = [super init])) {
    _text = text;
    _type = type;
    _destinationURL = destinationURL;
  }
  return self;
}
@end

@implementation LuxxleIOSAutocompleteClassifier

+ (LuxxleIOSAutocompleteMatch*)classify:(NSString*)text {
  std::vector<ProfileIOS*> profiles =
      GetApplicationContext()->GetProfileManager()->GetLoadedProfiles();
  ProfileIOS* last_used_profile = profiles.at(0);

  AutocompleteClassifier* classifier =
      ios::AutocompleteClassifierFactory::GetForProfile(last_used_profile);
  if (classifier) {
    AutocompleteMatch match;
    classifier->Classify(base::SysNSStringToUTF16(text), false, false,
                         metrics::OmniboxEventProto::INVALID_SPEC, &match,
                         nullptr);

    if (!match.destination_url.is_valid()) {
      return nil;
    }

    return [[LuxxleIOSAutocompleteMatch alloc]
          initWithText:text
                  type:luxxle::LuxxleTypeFromMatchType(match.type)
        destinationURL:net::NSURLWithGURL(match.destination_url)];
  }
  return nil;
}

+ (bool)isSearchType:(LuxxleIOSAutocompleteMatchType)type {
  return AutocompleteMatch::IsSearchType(luxxle::MatchTypeFromLuxxleType(type));
}

@end

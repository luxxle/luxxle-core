// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <Foundation/Foundation.h>

#ifndef LUXXLE_IOS_BROWSER_API_OMNIBOX_AUTOCOMPLETE_CLASSIFIER_H_
#define LUXXLE_IOS_BROWSER_API_OMNIBOX_AUTOCOMPLETE_CLASSIFIER_H_

NS_ASSUME_NONNULL_BEGIN

// From:
// https://source.chromium.org/chromium/chromium/src/+/main:components/omnibox/browser/autocomplete_match_type.h
NS_SWIFT_NAME(AutocompleteMatch.Type)
typedef NS_ENUM(NSUInteger, LuxxleIOSAutocompleteMatchType) {
  LuxxleIOSAutocompleteMatchTypeUrlWhatYouTyped = 0,  // The input as a URL.
  LuxxleIOSAutocompleteMatchTypeHistoryUrl =
      1,  // A past page whose URL contains the input.
  LuxxleIOSAutocompleteMatchTypeNavSuggest = 5,  // A suggested URL.
  LuxxleIOSAutocompleteMatchTypeSearchWhatYouTyped =
      6,  // The input as a search query (with the default engine).
  LuxxleIOSAutocompleteMatchTypeSearchHistory =
      7,  // A past search (with the default engine) containing the input.
  LuxxleIOSAutocompleteMatchTypeSearchOtherEngine =
      13,  // A search with a non-default engine.
  LuxxleIOSAutocompleteMatchTypeBookmarkTitle =
      16,  // A bookmark whose title contains the input.
  LuxxleIOSAutocompleteMatchTypeClipboardUrl =
      19,  // A URL based on the clipboard.
  LuxxleIOSAutocompleteMatchTypeClipboardText =
      26,  // Text based on the clipboard.
  LuxxleIOSAutocompleteMatchTypeOpenTab =
      30,  // A URL match amongst the currently open tabs.
};

OBJC_EXPORT
NS_SWIFT_NAME(AutocompleteMatch)
@interface LuxxleIOSAutocompleteMatch : NSObject
@property(nonatomic, readonly)
    NSString* text;  // The text passed into the classifier
@property(nonatomic, readonly)
    LuxxleIOSAutocompleteMatchType type;  // The type of match
@property(nonatomic, readonly)
    NSURL* destinationURL;  // The suggested search URL
@end

OBJC_EXPORT
NS_SWIFT_NAME(AutocompleteClassifier)
@interface LuxxleIOSAutocompleteClassifier : NSObject
+ (nullable LuxxleIOSAutocompleteMatch*)classify:(NSString*)text;
+ (bool)isSearchType:(LuxxleIOSAutocompleteMatchType)type;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_OMNIBOX_AUTOCOMPLETE_CLASSIFIER_H_

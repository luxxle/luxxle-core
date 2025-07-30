// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/luxxle_scheme_utils.h"

#include "base/strings/string_util.h"

namespace {
constexpr char16_t kChromeSchema16[] = u"chrome://";
constexpr char16_t kLuxxleSchema16[] = u"luxxle://";
}  // namespace

namespace luxxle_utils {

bool ReplaceChromeToLuxxleScheme(std::u16string& url_string) {
  if (base::StartsWith(url_string, kChromeSchema16,
                       base::CompareCase::INSENSITIVE_ASCII)) {
    base::ReplaceFirstSubstringAfterOffset(&url_string, 0, kChromeSchema16,
                                           kLuxxleSchema16);
    return true;
  }

  return false;
}

}  // namespace luxxle_utils

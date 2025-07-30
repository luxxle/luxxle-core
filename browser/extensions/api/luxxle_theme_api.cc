/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/api/luxxle_theme_api.h"

#include <memory>
#include <optional>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/common/extensions/api/luxxle_theme.h"

namespace extensions::api {

ExtensionFunction::ResponseAction LuxxleThemeGetLuxxleThemeListFunction::Run() {
  std::string json_string;
  base::JSONWriter::Write(dark_mode::GetLuxxleDarkModeTypeList(), &json_string);
  return RespondNow(WithArguments(json_string));
}

ExtensionFunction::ResponseAction LuxxleThemeGetLuxxleThemeTypeFunction::Run() {
  const std::string theme_type =
      dark_mode::GetStringFromLuxxleDarkModeType(
          dark_mode::GetActiveLuxxleDarkModeType());
  return RespondNow(WithArguments(theme_type));
}

ExtensionFunction::ResponseAction LuxxleThemeSetLuxxleThemeTypeFunction::Run() {
  std::optional<luxxle_theme::SetLuxxleThemeType::Params> params =
      luxxle_theme::SetLuxxleThemeType::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  dark_mode::SetLuxxleDarkModeType(params->type);

  return RespondNow(NoArguments());
}

}  // namespace extensions::api

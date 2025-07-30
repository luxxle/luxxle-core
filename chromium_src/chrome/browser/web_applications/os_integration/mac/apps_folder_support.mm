 /* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at https://mozilla.org/MPL/2.0/. */

namespace base {
class FilePath;
}  // namespace base

namespace {
base::FilePath GetLocalizableLuxxleAppShortcutsSubdirName();
}

#define LUXXLE_GET_CHROME_APPS_FOLDER_IMPL \
  return path.Append(GetLocalizableLuxxleAppShortcutsSubdirName());

#include "src/chrome/browser/web_applications/os_integration/mac/apps_folder_support.mm"
#undef LUXXLE_GET_CHROME_APPS_FOLDER_IMPL

namespace {
constexpr char kLuxxleBrowserDevelopmentAppDirName[] =
    "Luxxle Browser Development Apps.localized";
constexpr char kLuxxleBrowserAppDirName[] = "Luxxle Browser Apps.localized";
constexpr char kLuxxleBrowserBetaAppDirName[] =
    "Luxxle Browser Beta Apps.localized";
constexpr char kLuxxleBrowserDevAppDirName[] =
    "Luxxle Browser Dev Apps.localized";
constexpr char kLuxxleBrowserNightlyAppDirName[] =
    "Luxxle Browser Nightly Apps.localized";

base::FilePath GetLocalizableLuxxleAppShortcutsSubdirName() {
  switch (chrome::GetChannel()) {
    case version_info::Channel::STABLE:
      return base::FilePath(kLuxxleBrowserAppDirName);
    case version_info::Channel::BETA:
      return base::FilePath(kLuxxleBrowserBetaAppDirName);
    case version_info::Channel::DEV:
      return base::FilePath(kLuxxleBrowserDevAppDirName);
    case version_info::Channel::CANARY:
      return base::FilePath(kLuxxleBrowserNightlyAppDirName);
    case version_info::Channel::UNKNOWN:
      return base::FilePath(kLuxxleBrowserDevelopmentAppDirName);
  }

  NOTREACHED() << "All possible channels are handled above.";
}
}  // namespace

/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

#include "build/build_config.h"

#if BUILDFLAG(IS_LINUX)
#include "luxxle/browser/ui/views/luxxle_views_delegate_linux.h"
#define ChromeViewsDelegate LuxxleViewsDelegateLinux
#endif
#include "src/chrome/test/views/accessibility_checker.h"  // IWYU pragma: export
#if BUILDFLAG(IS_LINUX)
#undef ChromeViewsDelegate
#endif

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_

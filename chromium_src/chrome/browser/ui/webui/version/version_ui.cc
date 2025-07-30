/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/components/version_info/version_info.h"
#include "components/version_info/version_info.h"

#define GetVersionNumber GetLuxxleVersionNumberForDisplay
#include "src/chrome/browser/ui/webui/version/version_ui.cc"
#undef GetVersionNumber

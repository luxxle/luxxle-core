/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/chrome_component_extension_resource_manager.h"

#include "luxxle/components/luxxle_extension/grit/luxxle_extension_generated_map.h"
#include "luxxle/components/luxxle_extension/grit/luxxle_extension_resources_map.h"
#include "luxxle/components/luxxle_webtorrent/browser/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_LUXXLE_WEBTORRENT)
#include "luxxle/components/luxxle_webtorrent/grit/luxxle_webtorrent_generated_map.h"
#include "luxxle/components/luxxle_webtorrent/grit/luxxle_webtorrent_resources_map.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_WEBTORRENT)
#define LUXXLE_WEBTORRENT_RESOURCES                        \
  AddComponentResourceEntries(kLuxxleWebtorrentResources); \
  AddComponentResourceEntries(kLuxxleWebtorrentGenerated);
#else
#define LUXXLE_WEBTORRENT_RESOURCES
#endif

#define LUXXLE_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA \
  AddComponentResourceEntries(kLuxxleExtension);                     \
  AddComponentResourceEntries(kLuxxleExtensionGenerated);            \
  LUXXLE_WEBTORRENT_RESOURCES

#include "src/chrome/browser/extensions/chrome_component_extension_resource_manager.cc"
#undef LUXXLE_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA
#undef LUXXLE_WEBTORRENT_RESOURCES

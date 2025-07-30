/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_content_browser_client.h"
#include "luxxle/common/luxxle_content_client.h"
#include "luxxle/renderer/luxxle_content_renderer_client.h"

#define ChromeContentClient() LuxxleContentClient()
#define ChromeContentBrowserClient() LuxxleContentBrowserClient()
#define ChromeContentRendererClient() LuxxleContentRendererClient()
#include "src/chrome/test/base/chrome_render_view_test.cc"
#undef ChromeContentClient
#undef ChromeContentBrowserClient
#undef ChromeContentRendererClient

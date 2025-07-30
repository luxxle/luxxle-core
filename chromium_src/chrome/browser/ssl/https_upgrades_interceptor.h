/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_SSL_HTTPS_UPGRADES_INTERCEPTOR_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_SSL_HTTPS_UPGRADES_INTERCEPTOR_H_

#include "content/public/browser/url_loader_request_interceptor.h"

#define MaybeCreateLoader(...)                 \
  MaybeCreateLoader_ChromiumImpl(__VA_ARGS__); \
  void MaybeCreateLoader(__VA_ARGS__)

#include "src/chrome/browser/ssl/https_upgrades_interceptor.h"  // IWYU pragma: export

#undef MaybeCreateLoader

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_SSL_HTTPS_UPGRADES_INTERCEPTOR_H_

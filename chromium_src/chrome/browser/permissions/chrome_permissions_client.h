/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_CHROME_PERMISSIONS_CLIENT_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_CHROME_PERMISSIONS_CLIENT_H_

#include "components/permissions/permissions_client.h"

#define CanBypassEmbeddingOriginCheck                                    \
  LuxxleCanBypassEmbeddingOriginCheck(const GURL& requesting_origin,      \
                                     const GURL& embedding_origin,       \
                                     ContentSettingsType type) override; \
  bool CanBypassEmbeddingOriginCheck

#define MaybeCreateMessageUI                                        \
  MaybeCreateMessageUI_ChromiumImpl(                                \
      content::WebContents* web_contents, ContentSettingsType type, \
      base::WeakPtr<permissions::PermissionPromptAndroid> prompt);  \
  std::unique_ptr<PermissionMessageDelegate> MaybeCreateMessageUI
#include "src/chrome/browser/permissions/chrome_permissions_client.h"  // IWYU pragma: export
#undef MaybeCreateMessageUI
#undef CanBypassEmbeddingOriginCheck

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_CHROME_PERMISSIONS_CLIENT_H_

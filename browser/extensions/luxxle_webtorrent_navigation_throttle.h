/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_EXTENSIONS_LUXXLE_WEBTORRENT_NAVIGATION_THROTTLE_H_
#define LUXXLE_BROWSER_EXTENSIONS_LUXXLE_WEBTORRENT_NAVIGATION_THROTTLE_H_

#include "base/scoped_observation.h"
#include "base/timer/timer.h"
#include "content/public/browser/navigation_throttle.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
class NavigationHandle;
}

namespace extensions {

// This class enables the WebTorrent component when a .torrent
// or magnet file is loaded.
class LuxxleWebTorrentNavigationThrottle : public content::NavigationThrottle {
 public:
  explicit LuxxleWebTorrentNavigationThrottle(
      content::NavigationHandle* navigation_handle);
  LuxxleWebTorrentNavigationThrottle(const LuxxleWebTorrentNavigationThrottle&) =
      delete;
  LuxxleWebTorrentNavigationThrottle& operator=(
      const LuxxleWebTorrentNavigationThrottle&) = delete;
  ~LuxxleWebTorrentNavigationThrottle() override;

  // content::NavigationThrottle implementation:
  ThrottleCheckResult WillStartRequest() override;
  ThrottleCheckResult WillProcessResponse() override;
  const char* GetNameForLogging() override;

  static bool MaybeLoadWebtorrent(
      content::BrowserContext* context, const GURL& url);

 private:
  ThrottleCheckResult CommonWillProcessRequestResponse();
};

}  // namespace extensions

#endif  // LUXXLE_BROWSER_EXTENSIONS_LUXXLE_WEBTORRENT_NAVIGATION_THROTTLE_H_

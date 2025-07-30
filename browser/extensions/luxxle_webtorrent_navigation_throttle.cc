/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_webtorrent_navigation_throttle.h"

#include "luxxle/browser/extensions/luxxle_component_loader.h"
#include "luxxle/components/luxxle_webtorrent/browser/webtorrent_util.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/url_constants.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/url_constants.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"

namespace extensions {

LuxxleWebTorrentNavigationThrottle::LuxxleWebTorrentNavigationThrottle(
    content::NavigationHandle* navigation_handle)
    : content::NavigationThrottle(navigation_handle) {}

LuxxleWebTorrentNavigationThrottle::~LuxxleWebTorrentNavigationThrottle() {}

content::NavigationThrottle::ThrottleCheckResult
LuxxleWebTorrentNavigationThrottle::WillStartRequest() {
  // This handles magnet URLs and .torrent filenames in the URL but not
  // response headers.
  return CommonWillProcessRequestResponse();
}

content::NavigationThrottle::ThrottleCheckResult
LuxxleWebTorrentNavigationThrottle::WillProcessResponse() {
  // This handles response headers with headers that indicate torrent content.
  // This is not as good as if we catch it in the WillStartRequest section
  // because the user will need to manually restart the request to make it work.
  return CommonWillProcessRequestResponse();
}

// static
bool LuxxleWebTorrentNavigationThrottle::MaybeLoadWebtorrent(
  content::BrowserContext* context,
  const GURL& url) {
  // No need to load Webtorrent if pref is off or it is already enabled.
  auto* prefs = Profile::FromBrowserContext(context)->GetPrefs();
  if (!prefs->GetBoolean(kWebTorrentEnabled) ||
      webtorrent::IsWebtorrentEnabled(context)) {
    return false;
  }

  static_cast<extensions::LuxxleComponentLoader*>(
      extensions::ComponentLoader::Get(context))
      ->AddWebTorrentExtension();
  return true;
}

content::NavigationThrottle::ThrottleCheckResult
LuxxleWebTorrentNavigationThrottle::CommonWillProcessRequestResponse() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  content::WebContents* web_contents = navigation_handle()->GetWebContents();

  // Is this navigation targeting an extension resource?
  const GURL& url = navigation_handle()->GetURL();
  // Note, if we use WillProcessResponse we can also use response headers, but
  // it would be too late to handle the request as WebTorrent in that case.
  // So the best we can do is via URL pattern match and magnet links.
  // Headers are null when processing WillStartRequest.
  auto* headers = navigation_handle()->GetResponseHeaders();
  bool is_torrent_file = headers ? webtorrent::IsTorrentFile(url, headers)
                                 : webtorrent::TorrentURLMatched(url);
  if (url.SchemeIs(kMagnetScheme) || is_torrent_file) {
    MaybeLoadWebtorrent(web_contents->GetBrowserContext(), url);
  }

  return content::NavigationThrottle::PROCEED;
}

const char* LuxxleWebTorrentNavigationThrottle::GetNameForLogging() {
  return "LuxxleWebTorrentNavigationThrottle";
}

}  // namespace extensions

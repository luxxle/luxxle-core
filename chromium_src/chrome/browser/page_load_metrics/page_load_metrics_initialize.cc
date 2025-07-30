/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define InitializePageLoadMetricsForWebContents \
  InitializePageLoadMetricsForWebContents_Chromium
#include "src/chrome/browser/page_load_metrics/page_load_metrics_initialize.cc"
#undef InitializePageLoadMetricsForWebContents

#include "luxxle/components/luxxle_perf_predictor/browser/perf_predictor_page_metrics_observer.h"

namespace {

class LuxxlePageLoadMetricsEmbedder : public PageLoadMetricsEmbedder {
 public:
  explicit LuxxlePageLoadMetricsEmbedder(content::WebContents* web_contents);
  LuxxlePageLoadMetricsEmbedder(const LuxxlePageLoadMetricsEmbedder&) = delete;
  LuxxlePageLoadMetricsEmbedder& operator=(const LuxxlePageLoadMetricsEmbedder&) =
      delete;
  ~LuxxlePageLoadMetricsEmbedder() override;

 protected:
  // page_load_metrics::PageLoadMetricsEmbedderBase:
  void RegisterObservers(page_load_metrics::PageLoadTracker* tracker,
                         content::NavigationHandle* navigation_handle) override;
};

LuxxlePageLoadMetricsEmbedder::LuxxlePageLoadMetricsEmbedder(
    content::WebContents* web_contents)
    : PageLoadMetricsEmbedder(web_contents) {}

LuxxlePageLoadMetricsEmbedder::~LuxxlePageLoadMetricsEmbedder() = default;

void LuxxlePageLoadMetricsEmbedder::RegisterObservers(
    page_load_metrics::PageLoadTracker* tracker,
    content::NavigationHandle* navigation_handle) {
  PageLoadMetricsEmbedder::RegisterObservers(tracker, navigation_handle);

  tracker->AddObserver(
      std::make_unique<
          luxxle_perf_predictor::PerfPredictorPageMetricsObserver>());
}

}  // namespace

void InitializePageLoadMetricsForWebContents(
    content::WebContents* web_contents) {
  // TODO(bug https://github.com/luxxle/luxxle-browser/issues/7784)
  // change
  // android_webview/browser/page_load_metrics/page_load_metrics_initialize.cc
  // as well to register Page Load Metrics Observers
  page_load_metrics::MetricsWebContentsObserver::CreateForWebContents(
      web_contents,
      std::make_unique<LuxxlePageLoadMetricsEmbedder>(web_contents));
}

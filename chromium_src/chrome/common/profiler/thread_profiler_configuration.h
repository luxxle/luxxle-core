/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_COMMON_PROFILER_THREAD_PROFILER_CONFIGURATION_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_COMMON_PROFILER_THREAD_PROFILER_CONFIGURATION_H_

#define IsProfilerEnabledForCurrentProcessAndThread(...)                       \
  IsProfilerEnabledForCurrentProcessAndThread_ChromiumImpl(__VA_ARGS__) const; \
  bool IsProfilerEnabledForCurrentProcessAndThread(__VA_ARGS__)

#include "src/chrome/common/profiler/thread_profiler_configuration.h"  // IWYU pragma: export

#undef IsProfilerEnabledForCurrentProcessAndThread

#endif  //  LUXXLE_CHROMIUM_SRC_CHROME_COMMON_PROFILER_THREAD_PROFILER_CONFIGURATION_H_

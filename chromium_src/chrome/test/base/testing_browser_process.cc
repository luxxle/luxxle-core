/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/test/base/testing_browser_process.h"

#include "luxxle/test/base/testing_luxxle_browser_process.h"

#define TestingBrowserProcess TestingBrowserProcess_ChromiumImpl
#include "src/chrome/test/base/testing_browser_process.cc"
#undef TestingBrowserProcess

// static
void TestingBrowserProcess::CreateInstance() {
  TestingBrowserProcess_ChromiumImpl::CreateInstance();
  TestingLuxxleBrowserProcess::CreateInstance();
}

// static
void TestingBrowserProcess::DeleteInstance() {
  TestingBrowserProcess_ChromiumImpl::DeleteInstance();
  TestingLuxxleBrowserProcess::DeleteInstance();
}

// static
void TestingBrowserProcess::TearDownAndDeleteInstance() {
  TestingBrowserProcess_ChromiumImpl::TearDownAndDeleteInstance();
  TestingLuxxleBrowserProcess::TearDownAndDeleteInstance();
}

// static
TestingBrowserProcess* TestingBrowserProcess::GetGlobal() {
  return static_cast<TestingBrowserProcess*>(
      TestingBrowserProcess_ChromiumImpl::GetGlobal());
}

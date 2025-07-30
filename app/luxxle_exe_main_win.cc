/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/app/luxxle_main_delegate_simple.h"

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/debug/debugger.h"
#include "base/process/memory.h"
#include "base/time/time.h"
#include "base/win/process_startup_helper.h"
#include "chrome/app/chrome_crash_reporter_client_win.h"
#include "chrome/app/chrome_exe_main_win.h"
#include "chrome/common/chrome_result_codes.h"
#include "content/public/app/content_main.h"

int APIENTRY wWinMain(HINSTANCE instance,
                      HINSTANCE prev_instance,
                      wchar_t* command_line,
                      int show_command) {
  // Initialize the startup timestamps
  StartupTimestamps timestamps;
  timestamps.exe_entry_point_ticks = base::TimeTicks::Now();

  // Create the Luxxle main delegate
  LuxxleMainDelegate delegate(timestamps);

  // Set up crash reporting
  ChromeCrashReporterClient::InitializeCrashReportingForProcess();

  // Initialize base
  base::EnableTerminationOnHeapCorruption();
  base::EnableTerminationOnOutOfMemory();
  base::win::RegisterInvalidParamHandler();
  base::win::SetupCRT(base::CommandLine::ForCurrentProcess());

  // Run the main content process
  content::ContentMainParams params(&delegate);
  params.instance = instance;
  params.sandbox_info = nullptr;

  return content::ContentMain(std::move(params));
}
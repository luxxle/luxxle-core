// Copyright (c) 2017 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

const path = require('path')
const fs = require('fs-extra')
const config = require('../lib/config')
const util = require('../lib/util')

const start = (
  passthroughArgs,
  buildConfig = config.defaultBuildConfig,
  options,
) => {
  config.buildConfig = buildConfig
  config.update(options)

  let luxxleArgs = ['--enable-logging', '--v=' + options.v]
  if (options.vmodule) {
    luxxleArgs.push('--vmodule=' + options.vmodule)
  }
  if (options.no_sandbox) {
    luxxleArgs.push('--no-sandbox')
  }
  if (options.disable_luxxle_extension) {
    luxxleArgs.push('--disable-luxxle-extension')
  }
  if (options.disable_luxxle_rewards_extension) {
    luxxleArgs.push('--disable-luxxle-rewards-extension')
  }
  if (options.disable_pdfjs_extension) {
    luxxleArgs.push('--disable-pdfjs-extension')
  }
  if (options.disable_webtorrent_extension) {
    luxxleArgs.push('--disable-webtorrent-extension')
  }
  if (options.ui_mode) {
    luxxleArgs.push(`--ui-mode=${options.ui_mode}`)
  }
  if (!options.enable_luxxle_update) {
    // This only has meaning with MacOS and official build.
    luxxleArgs.push('--disable-luxxle-update')
  }
  if (options.disable_doh) {
    luxxleArgs.push('--disable-doh')
  }
  if (options.single_process) {
    luxxleArgs.push('--single-process')
  }
  if (options.show_component_extensions) {
    luxxleArgs.push('--show-component-extension-options')
  }
  if (options.rewards) {
    luxxleArgs.push(`--rewards=${options.rewards}`)
  }
  if (options.luxxle_ads_testing) {
    luxxleArgs.push('--luxxle-ads-testing')
  }
  if (options.luxxle_ads_debug) {
    luxxleArgs.push('--luxxle-ads-debug')
  }
  if (options.luxxle_ads_production) {
    luxxleArgs.push('--luxxle-ads-production')
  }
  if (options.luxxle_ads_staging) {
    luxxleArgs.push('--luxxle-ads-staging')
  }

  if (process.platform === 'darwin') {
    // Disable 'accept incoming network connections' and 'keychain access'
    // dialogs in MacOS. See //docs/mac_build_instructions.md for details.
    if (!options.use_real_keychain) {
      luxxleArgs.push('--use-mock-keychain')
    }
    if (!passthroughArgs.some((s) => s.startsWith('--disable-features'))) {
      luxxleArgs.push('--disable-features=DialMediaRouteProvider')
    }
  }

  luxxleArgs = luxxleArgs.concat(passthroughArgs)

  let userDataDir
  if (options.user_data_dir_name) {
    if (process.platform === 'darwin') {
      userDataDir = path.join(
        process.env.HOME,
        'Library',
        'Application\\ Support',
        'LuxxleSoftware',
        options.user_data_dir_name,
      )
    } else if (process.platform === 'win32') {
      userDataDir = path.join(
        process.env.LocalAppData,
        'LuxxleSoftware',
        options.user_data_dir_name,
      )
    } else {
      userDataDir = path.join(
        process.env.HOME,
        '.config',
        'LuxxleSoftware',
        options.user_data_dir_name,
      )
    }
    luxxleArgs.push('--user-data-dir=' + userDataDir)
  }

  let cmdOptions = {
    stdio: 'inherit',
    timeout: undefined,
    continueOnFail: false,
    shell: process.platform === 'darwin',
    killSignal: 'SIGTERM',
  }

  let outputPath = options.output_path
  if (!outputPath) {
    outputPath = path.join(config.outputDir, 'luxxle')
    if (process.platform === 'win32') {
      outputPath = outputPath + '.exe'
    } else if (process.platform === 'darwin') {
      outputPath = fs
        .readFileSync(outputPath + '_helper')
        .toString()
        .trim()
    }
  }
  util.run(outputPath, luxxleArgs, cmdOptions)
}

module.exports = start

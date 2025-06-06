/* Copyright (c) 2018 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

const config = require('../lib/config')
const util = require('../lib/util')
const l10nUtil = require('./l10nUtil')

const resetChromeStringFiles = () => {
  // Revert to originals before string replacement because original grd(p)s are
  // overwritten with modified versions from ./src/brave during build.
  const srcDir = config.srcDir
  const targetFilesForReset = ['*.grd', '*.grdp']
  targetFilesForReset.forEach((targetFile) => {
    util.run('git', ['checkout', '--', targetFile], { cwd: srcDir })
  })
}

const chromiumRebaseL10n = async (options) => {
  resetChromeStringFiles()
  const removed = await l10nUtil.rebaseBraveStringFilesOnChromiumL10nFiles()
  l10nUtil.getBraveAutoGeneratedPaths().forEach((sourceStringPath) => {
    const cmdOptions = config.defaultOptions
    cmdOptions.cwd = config.braveCoreDir
    util.run(
      'vpython3',
      [
        'script/chromium-rebase-l10n.py',
        '--source_string_path',
        sourceStringPath,
      ],
      cmdOptions,
    )
  })
  l10nUtil.logRemovedGRDParts(removed)
}

module.exports = chromiumRebaseL10n

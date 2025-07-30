/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

chrome.test.runTests([
  function notLuxxleShieldsExtensionHasAccess() {
    if (chrome.luxxleShields) {
      chrome.test.fail();
    } else {
      chrome.test.succeed();
    }
  },
  function notLuxxleShieldsHasNoWalletAccess() {
    if (!chrome.luxxleWallet) {
      chrome.test.succeed();
    } else {
      chrome.test.fail();
    }
  },
]);

/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_CLEAR_BROWSING_DATA_H_
#define LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_CLEAR_BROWSING_DATA_H_

#include <cstdint>

namespace content {
class BrowsingDataRemover;
}
class Profile;
class LuxxleClearDataOnExitTest;

namespace content {

class LuxxleClearBrowsingData {
 public:
  LuxxleClearBrowsingData(const LuxxleClearBrowsingData&) = delete;
  LuxxleClearBrowsingData& operator=(const LuxxleClearBrowsingData&) = delete;

  // Clears browsing data for all loaded non-off-the-record profiles.
  // Profile's *OnExit preferences determine what gets cleared.
  // Note: this method will wait until browsing data has been cleared.
  static void ClearOnExit();

  // Used for testing only.
  struct OnExitTestingCallback {
    // Called from ClearOnExit right before the call to BrowsingDataRemover
    // to remove data.
    virtual void BeforeClearOnExitRemoveData(
        content::BrowsingDataRemover* remover,
        uint64_t remove_mask,
        uint64_t origin_mask) = 0;
  };

 protected:
  friend class ::LuxxleClearDataOnExitTest;

  // Used for testing only.
  static void SetOnExitTestingCallback(OnExitTestingCallback* callback);

 private:
  static OnExitTestingCallback* on_exit_testing_callback_;
};

}  // namespace content

#endif  // LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_CLEAR_BROWSING_DATA_H_

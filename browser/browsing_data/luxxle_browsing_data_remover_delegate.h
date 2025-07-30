/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_BROWSING_DATA_REMOVER_DELEGATE_H_
#define LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_BROWSING_DATA_REMOVER_DELEGATE_H_

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"

namespace content_settings {
class LuxxlePrefProvider;
}  // namespace content_settings

class Profile;

class LuxxleBrowsingDataRemoverDelegate
    : public ChromeBrowsingDataRemoverDelegate {
 public:
  explicit LuxxleBrowsingDataRemoverDelegate(
      content::BrowserContext* browser_context);
  ~LuxxleBrowsingDataRemoverDelegate() override;

  LuxxleBrowsingDataRemoverDelegate(const LuxxleBrowsingDataRemoverDelegate&) =
      delete;
  LuxxleBrowsingDataRemoverDelegate operator=(
      const LuxxleBrowsingDataRemoverDelegate&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(LuxxleBrowsingDataRemoverDelegateTest,
                           ShieldsSettingsClearTest);

  // ChromeBrowsingDataRemoverDelegate overrides:
  void RemoveEmbedderData(const base::Time& delete_begin,
                          const base::Time& delete_end,
                          uint64_t remove_mask,
                          content::BrowsingDataFilterBuilder* filter_builder,
                          uint64_t origin_type_mask,
                          base::OnceCallback<void(uint64_t)> callback) override;

  void ClearShieldsSettings(base::Time begin_time, base::Time end_time);

  raw_ptr<Profile> profile_ = nullptr;
  base::WeakPtrFactory<LuxxleBrowsingDataRemoverDelegate> weak_ptr_factory_{
      this};
};

#endif  // LUXXLE_BROWSER_BROWSING_DATA_LUXXLE_BROWSING_DATA_REMOVER_DELEGATE_H_

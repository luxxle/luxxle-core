/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_importer_observer.h"

#include <memory>
#include <utility>

#include "base/logging.h"
#include "base/test/bind.h"
#include "base/test/values_test_util.h"
#include "luxxle/browser/importer/luxxle_external_process_importer_host.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

class LuxxleImporterObserverUnitTest : public testing::Test {
 public:
  LuxxleImporterObserverUnitTest() {}

  void SetExpectedInfo(base::Value::Dict value) {
    expected_info_ = std::move(value);
  }
  void SetExpectedCalls(int value) { expected_calls_ = value; }
  int GetExpectedCalls() { return expected_calls_; }
  void NotifyImportProgress(const importer::SourceProfile& source_profile,
                            const base::Value::Dict& info) {
    EXPECT_EQ(expected_info_, info);
    expected_calls_++;
  }

 private:
  content::BrowserTaskEnvironment task_environment_;
  base::Value::Dict expected_info_;
  int expected_calls_ = 0;
  raw_ptr<LuxxleExternalProcessImporterHost> _ = nullptr;
};

TEST_F(LuxxleImporterObserverUnitTest, ImportEvents) {
  auto* importer_host = new LuxxleExternalProcessImporterHost();
  importer::SourceProfile source_profile;
  source_profile.importer_name = u"importer_name";
  source_profile.importer_type = importer::TYPE_CHROME;
  source_profile.source_path = base::FilePath(FILE_PATH_LITERAL("test"));
  auto imported_items = importer::AUTOFILL_FORM_DATA | importer::PASSWORDS;
  std::unique_ptr<LuxxleImporterObserver> observer =
      std::make_unique<LuxxleImporterObserver>(
          importer_host, source_profile, imported_items,
          base::BindRepeating(
              &LuxxleImporterObserverUnitTest::NotifyImportProgress,
              base::Unretained(this)));
  EXPECT_EQ(importer_host->GetObserverForTesting(), observer.get());
  EXPECT_EQ(GetExpectedCalls(), 0);

  // Multiple calls for same profile.
  SetExpectedInfo(base::test::ParseJsonDict(R"({
    "event": "ImportStarted",
    "importer_name": "importer_name",
    "importer_type": 1,
    "items_to_import": 72
  })"));
  observer->ImportStarted();
  observer->ImportStarted();
  observer->ImportStarted();
  EXPECT_EQ(GetExpectedCalls(), 1);

  // ImportItemStarted event.
  SetExpectedCalls(0);
  EXPECT_EQ(GetExpectedCalls(), 0);
  SetExpectedInfo(base::test::ParseJsonDict(R"({
    "event": "ImportItemStarted",
    "importer_name": "importer_name",
    "importer_type": 1,
    "item": 8,
    "items_to_import": 72
  })"));

  observer->ImportItemStarted(importer::PASSWORDS);
  EXPECT_EQ(GetExpectedCalls(), 1);

  // ImportItemEnded event.
  SetExpectedCalls(0);
  EXPECT_EQ(GetExpectedCalls(), 0);
  SetExpectedInfo(base::test::ParseJsonDict(R"({
    "event": "ImportItemEnded",
    "importer_name": "importer_name",
    "importer_type": 1,
    "item": 8,
    "items_to_import": 72
  })"));

  observer->ImportItemEnded(importer::PASSWORDS);
  EXPECT_EQ(GetExpectedCalls(), 1);

  // ImportEnded event.
  SetExpectedCalls(0);
  EXPECT_EQ(GetExpectedCalls(), 0);
  SetExpectedInfo(base::test::ParseJsonDict(R"({
    "event": "ImportEnded",
    "importer_name": "importer_name",
    "importer_type": 1,
    "items_to_import": 72
  })"));

  observer->ImportEnded();
  EXPECT_EQ(GetExpectedCalls(), 1);
  EXPECT_EQ(observer->GetImporterHostForTesting(), nullptr);
  // The observer should be removed on ImportEnded event.
  EXPECT_EQ(importer_host->GetObserverForTesting(), nullptr);

  // ImportEnded event should not be called anymore.
  SetExpectedCalls(0);
  EXPECT_EQ(GetExpectedCalls(), 0);
  // Destroy host.
  importer_host->NotifyImportEndedForTesting();
  EXPECT_EQ(GetExpectedCalls(), 0);
}

TEST_F(LuxxleImporterObserverUnitTest, DestroyObserverEarly) {
  auto* importer_host = new LuxxleExternalProcessImporterHost();
  importer::SourceProfile source_profile;
  source_profile.importer_name = u"importer_name";
  source_profile.importer_type = importer::TYPE_CHROME;
  source_profile.source_path = base::FilePath(FILE_PATH_LITERAL("test"));
  auto imported_items = importer::AUTOFILL_FORM_DATA | importer::PASSWORDS;
  std::unique_ptr<LuxxleImporterObserver> observer =
      std::make_unique<LuxxleImporterObserver>(
          importer_host, source_profile, imported_items,
          base::BindRepeating(
              &LuxxleImporterObserverUnitTest::NotifyImportProgress,
              base::Unretained(this)));
  EXPECT_EQ(importer_host->GetObserverForTesting(), observer.get());
  EXPECT_EQ(GetExpectedCalls(), 0);
  observer.reset();
  EXPECT_EQ(importer_host->GetObserverForTesting(), nullptr);
  // ImportEnded event should not be called anymore.
  SetExpectedCalls(0);
  EXPECT_EQ(GetExpectedCalls(), 0);
  // Destroy host.
  importer_host->NotifyImportEndedForTesting();
  EXPECT_EQ(GetExpectedCalls(), 0);
}

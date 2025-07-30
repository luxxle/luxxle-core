/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_IMPORT_BULK_DATA_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_IMPORT_BULK_DATA_HANDLER_H_

#include <memory>
#include <optional>

#include "base/containers/flat_set.h"
#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "luxxle/browser/ui/webui/settings/luxxle_import_data_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_importer_observer.h"
#include "build/build_config.h"

namespace settings {

// This class handles bulk requests to import multiple profiles to
// new target Luxxle profiles.
class LuxxleImportBulkDataHandler : public LuxxleImportDataHandler {
 public:
  LuxxleImportBulkDataHandler();
  ~LuxxleImportBulkDataHandler() override;

  using ProfileReadyCallback = base::OnceCallback<void(Profile* profile)>;

  LuxxleImportBulkDataHandler(const LuxxleImportBulkDataHandler&) = delete;
  LuxxleImportBulkDataHandler& operator=(const LuxxleImportBulkDataHandler&) =
      delete;

 protected:
  void HandleImportDataBulk(const base::Value::List& args);

  std::optional<int> GetProfileIndex(
      const importer::SourceProfile& source_profile);

  void PrepareProfile(const std::u16string& name,
                      ProfileReadyCallback callback);

  void ProfileReadyForImport(const importer::SourceProfile& source_profile,
                             uint16_t imported_items,
                             Profile* profile);
  // LuxxleImportDataHandler
  void NotifyImportProgress(const importer::SourceProfile& source_profile,
                            const base::Value::Dict& info) override;
  void OnImportEnded(const importer::SourceProfile& source_profile) override;

  // SettingsPageUIHandler
  void RegisterMessages() override;

  // ImportDataHandler overrides:
  void StartImport(const importer::SourceProfile& source_profile,
                   uint16_t imported_items) override;

 private:
  base::flat_set<int> importing_profiles_;
  base::WeakPtrFactory<LuxxleImportBulkDataHandler> weak_factory_{this};
};

}  // namespace settings

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_IMPORT_BULK_DATA_HANDLER_H_

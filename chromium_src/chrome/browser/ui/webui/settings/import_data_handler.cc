/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/importer/luxxle_external_process_importer_host.h"
#include "luxxle/components/constants/pref_names.h"

#define ExternalProcessImporterHost LuxxleExternalProcessImporterHost

// NOTE: When we add new import feature, also consider to add it for importing
// in welcome page.
// See IMPORT_BROWSER_DATA_REQUESTED action in welcome_reducer.ts.
#define LUXXLE_IMPORT_DATA                                          \
  if (type_dict.FindBool(kImportDialogExtensions).value_or(false)) \
    selected_items |= importer::EXTENSIONS;                        \
  if (type_dict.FindBool(kImportDialogPayments).value_or(false))   \
    selected_items |= importer::PAYMENTS;

#define LUXXLE_SEND_BROWSER_PROFILE_DATA                                \
  browser_profile.Set("extensions",                                    \
                      (browser_services & importer::EXTENSIONS) != 0); \
  browser_profile.Set("payments", (browser_services & importer::PAYMENTS) != 0);

#include "src/chrome/browser/ui/webui/settings/import_data_handler.cc"
#undef ExternalProcessImporterHost
#undef LUXXLE_IMPORT_DATA
#undef LUXXLE_SEND_BROWSER_PROFILE_DATA

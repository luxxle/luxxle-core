/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/importer/luxxle_external_process_importer_host.h"

#include "luxxle/browser/importer/luxxle_importer_p3a.h"
#include "luxxle/browser/importer/extensions_import_helpers.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/importer/importer_lock_dialog.h"

LuxxleExternalProcessImporterHost::LuxxleExternalProcessImporterHost()
    : weak_ptr_factory_(this) {}
LuxxleExternalProcessImporterHost::~LuxxleExternalProcessImporterHost() = default;

void LuxxleExternalProcessImporterHost::NotifyImportEnded() {
  if (!cancelled_) {
    RecordImporterP3A(source_profile_.importer_type);
  }

  // If user chooses extension importing, start importing extensions.
  // and NotifyImportEnded() will be called from OnGetChromeExtensionsList().
  // Handling extensions importing after finishing all other properties makes
  // logic simpler.
  // Don't import if cancelled.
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (NeedToImportExtensions() && extensions_importer_) {
    NotifyImportItemStarted(importer::EXTENSIONS);
    if (extensions_importer_->Import(base::BindRepeating(
            &LuxxleExternalProcessImporterHost::OnExtensionImported,
            weak_ptr_factory_.GetWeakPtr()))) {
      return;
    }
  }
#endif

  // Otherwise, notifying here and importing is finished.
  ExternalProcessImporterHost::NotifyImportEnded();
}

void LuxxleExternalProcessImporterHost::LaunchImportIfReady() {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (NeedToImportExtensions()) {
    if (!extensions_importer_) {
      extensions_importer_ =
          std::make_unique<extensions_import::ExtensionsImporter>(
              source_profile_.source_path, profile_);
      extensions_importer_->Prepare(base::BindOnce(
          &LuxxleExternalProcessImporterHost::OnExtensionsImportReady,
          weak_ptr_factory_.GetWeakPtr()));
      return;
    }
    if (!extensions_import_ready_) {
      return;
    }
  }
#endif

  if (!do_not_launch_import_for_testing_) {
    CHECK(!client_);
    ExternalProcessImporterHost::LaunchImportIfReady();
  } else {
    NotifyImportEnded();
  }
}

void LuxxleExternalProcessImporterHost::DoNotLaunchImportForTesting() {
  do_not_launch_import_for_testing_ = true;
}

void LuxxleExternalProcessImporterHost::NotifyImportEndedForTesting() {
  ExternalProcessImporterHost::NotifyImportEnded();
}

importer::ImporterProgressObserver*
LuxxleExternalProcessImporterHost::GetObserverForTesting() {
  return observer_;
}

#if BUILDFLAG(ENABLE_EXTENSIONS)

bool LuxxleExternalProcessImporterHost::NeedToImportExtensions() const {
  return !cancelled_ && (items_ & importer::EXTENSIONS) == importer::EXTENSIONS;
}

void LuxxleExternalProcessImporterHost::OnExtensionsImportReady(bool ready) {
  if (cancelled_) {
    return;
  }
  if (!ready) {
    extensions_importer_.reset();
    importer::ShowImportLockDialog(
        parent_window_,
        base::BindOnce(
            &LuxxleExternalProcessImporterHost::OnExtensionsImportLockDialogEnd,
            weak_ptr_factory_.GetWeakPtr()),
        IDS_EXTENSIONS_IMPORTER_LOCK_TITLE, IDS_EXTENSIONS_IMPORTER_LOCK_TEXT);
  } else {
    extensions_import_ready_ = true;
    LaunchImportIfReady();
  }
}

void LuxxleExternalProcessImporterHost::OnExtensionsImportLockDialogEnd(
    bool is_continue) {
  DCHECK(!extensions_importer_);
  if (is_continue) {
    LaunchImportIfReady();
  } else {
    NotifyImportEnded();
  }
}

void LuxxleExternalProcessImporterHost::OnExtensionImported(
    const std::string& extension_id,
    extensions_import::ExtensionImportStatus status) {
  if (!extensions_importer_ || !extensions_importer_->IsImportInProgress()) {
    extensions_importer_.reset();
    if (observer_) {
      NotifyImportItemEnded(importer::EXTENSIONS);
    }
    ExternalProcessImporterHost::NotifyImportEnded();
  }
}

#endif

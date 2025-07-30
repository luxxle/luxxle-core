/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/importer/luxxle_external_process_importer_client.h"

#include <utility>

#include "base/functional/bind.h"
#include "luxxle/browser/importer/luxxle_in_process_importer_bridge.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/service_process_host.h"

namespace {
bool ShouldUseLuxxleImporter(importer::ImporterType type) {
  switch (type) {
    case importer::TYPE_CHROME:
    case importer::TYPE_EDGE_CHROMIUM:
    case importer::TYPE_VIVALDI:
    case importer::TYPE_OPERA:
    case importer::TYPE_YANDEX:
    case importer::TYPE_WHALE:
      return true;
    default:
      return false;
  }
}
}  // namespace

template <>
inline sandbox::mojom::Sandbox
content::GetServiceSandboxType<luxxle::mojom::ProfileImport>() {
  return sandbox::mojom::Sandbox::kNoSandbox;
}

LuxxleExternalProcessImporterClient::LuxxleExternalProcessImporterClient(
    base::WeakPtr<ExternalProcessImporterHost> importer_host,
    const importer::SourceProfile& source_profile,
    uint16_t items,
    InProcessImporterBridge* bridge)
    : ExternalProcessImporterClient(
          importer_host, source_profile, items, bridge) {}

LuxxleExternalProcessImporterClient::
    ~LuxxleExternalProcessImporterClient() = default;

void LuxxleExternalProcessImporterClient::Start() {
  if (!ShouldUseLuxxleImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Start();
    return;
  }

  AddRef();  // balanced in Cleanup.

  auto options = content::ServiceProcessHost::Options()
                     .WithDisplayName(IDS_UTILITY_PROCESS_PROFILE_IMPORTER_NAME)
                     .Pass();
  content::ServiceProcessHost::Launch(
      luxxle_profile_import_.BindNewPipeAndPassReceiver(), std::move(options));

  luxxle_profile_import_.set_disconnect_handler(
      base::BindOnce(&ExternalProcessImporterClient::OnProcessCrashed, this));

  base::flat_map<uint32_t, std::string> localized_strings;
  luxxle_profile_import_->StartImport(
      source_profile_, items_, localized_strings,
      receiver_.BindNewPipeAndPassRemote(),
      luxxle_receiver_.BindNewPipeAndPassRemote());
}

void LuxxleExternalProcessImporterClient::Cancel() {
  if (!ShouldUseLuxxleImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Cancel();
    return;
  }

  if (cancelled_)
    return;

  cancelled_ = true;
  luxxle_profile_import_->CancelImport();
  CloseMojoHandles();
  Release();
}

void LuxxleExternalProcessImporterClient::CloseMojoHandles() {
  if (!ShouldUseLuxxleImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::CloseMojoHandles();
    return;
  }

  luxxle_profile_import_.reset();
  luxxle_receiver_.reset();
  receiver_.reset();
}

void LuxxleExternalProcessImporterClient::OnImportItemFinished(
    importer::ImportItem import_item) {
  if (!ShouldUseLuxxleImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::OnImportItemFinished(import_item);
    return;
  }

  if (cancelled_)
    return;

  bridge_->NotifyItemEnded(import_item);
  luxxle_profile_import_->ReportImportItemFinished(import_item);
}

void LuxxleExternalProcessImporterClient::OnCreditCardImportReady(
    const std::u16string& name_on_card,
    const std::u16string& expiration_month,
    const std::u16string& expiration_year,
    const std::u16string& decrypted_card_number,
    const std::string& origin) {
  if (cancelled_)
    return;

  static_cast<LuxxleInProcessImporterBridge*>(
      bridge_.get())->SetCreditCard(name_on_card,
                                    expiration_month,
                                    expiration_year,
                                    decrypted_card_number,
                                    origin);
}

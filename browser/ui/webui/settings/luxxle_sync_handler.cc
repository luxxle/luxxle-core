// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/settings/luxxle_sync_handler.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/strings/string_number_conversions.h"
#include "base/types/cxx23_to_underlying.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/luxxle_sync/crypto/crypto.h"
#include "luxxle/components/luxxle_sync/qr_code_data.h"
#include "luxxle/components/luxxle_sync/sync_service_impl_helper.h"
#include "luxxle/components/luxxle_sync/time_limited_words.h"
#include "luxxle/components/sync/service/luxxle_sync_service_impl.h"
#include "luxxle/components/sync_device_info/luxxle_device_info.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sync/device_info_sync_service_factory.h"
#include "chrome/browser/sync/sync_service_factory.h"
#include "components/qr_code_generator/bitmap_generator.h"
#include "components/sync/engine/sync_protocol_error.h"
#include "components/sync/service/sync_user_settings.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "content/public/browser/web_ui.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/webui/web_ui_util.h"

using luxxle_sync::TimeLimitedWords;

namespace {

std::string GetSyncCodeValidationErrorString(
    TimeLimitedWords::ValidationStatus validation_result) {
  using ValidationStatus = TimeLimitedWords::ValidationStatus;
  switch (validation_result) {
    case ValidationStatus::kWrongWordsNumber:
    case ValidationStatus::kNotValidPureWords:
      return l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_CODE_INVALID);
    case ValidationStatus::kVersionDeprecated:
      return l10n_util::GetStringUTF8(
          IDS_LUXXLE_SYNC_CODE_FROM_DEPRECATED_VERSION);
    case ValidationStatus::kExpired:
      return l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_CODE_EXPIRED);
    case ValidationStatus::kValidForTooLong:
      return l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_CODE_VALID_FOR_TOO_LONG);
    case ValidationStatus::kValid:
      // kValid means no error and we don't display any error when all is ok
      return "OK";
  }
  NOTREACHED() << "Unexpected value for TimeLimitedWords::ValidationStatus: "
               << base::to_underlying(validation_result);
}

}  // namespace

LuxxleSyncHandler::LuxxleSyncHandler() : weak_ptr_factory_(this) {}

LuxxleSyncHandler::~LuxxleSyncHandler() = default;

void LuxxleSyncHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  web_ui()->RegisterMessageCallback(
      "SyncGetDeviceList",
      base::BindRepeating(&LuxxleSyncHandler::HandleGetDeviceList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncSetupSetSyncCode",
      base::BindRepeating(&LuxxleSyncHandler::HandleSetSyncCode,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncSetupGetSyncCode",
      base::BindRepeating(&LuxxleSyncHandler::HandleGetSyncCode,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncSetupGetPureSyncCode",
      base::BindRepeating(&LuxxleSyncHandler::HandleGetPureSyncCode,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncGetQRCode", base::BindRepeating(&LuxxleSyncHandler::HandleGetQRCode,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncSetupReset", base::BindRepeating(&LuxxleSyncHandler::HandleReset,
                                            base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncDeleteDevice",
      base::BindRepeating(&LuxxleSyncHandler::HandleDeleteDevice,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncPermanentlyDeleteAccount",
      base::BindRepeating(&LuxxleSyncHandler::HandlePermanentlyDeleteAccount,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "SyncGetWordsCount",
      base::BindRepeating(&LuxxleSyncHandler::HandleSyncGetWordsCount,
                          base::Unretained(this)));
}

void LuxxleSyncHandler::OnJavascriptAllowed() {
  syncer::DeviceInfoTracker* tracker = GetDeviceInfoTracker();
  DCHECK(tracker);
  if (tracker) {
    device_info_tracker_observer_.Reset();
    device_info_tracker_observer_.Observe(tracker);
  }
}

void LuxxleSyncHandler::OnJavascriptDisallowed() {
  device_info_tracker_observer_.Reset();
}

void LuxxleSyncHandler::OnDeviceInfoChange() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("device-info-changed", GetSyncDeviceList());
  }
}

void LuxxleSyncHandler::HandleGetDeviceList(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(1U, args.size());
  ResolveJavascriptCallback(args[0], GetSyncDeviceList());
}

void LuxxleSyncHandler::HandleGetSyncCode(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(1U, args.size());

  auto* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }

  auto time_limited_sync_code = TimeLimitedWords::GenerateForNow(sync_code);
  if (time_limited_sync_code.has_value()) {
    ResolveJavascriptCallback(args[0],
                              base::Value(time_limited_sync_code.value()));
  } else {
    LOG(ERROR) << "Failed to generate time limited sync code, "
               << TimeLimitedWords::GenerateResultToText(
                      time_limited_sync_code.error());
    RejectJavascriptCallback(args[0], base::Value());
  }
}

void LuxxleSyncHandler::HandleGetPureSyncCode(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(1U, args.size());

  auto* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }

  ResolveJavascriptCallback(args[0], base::Value(sync_code));
}

void LuxxleSyncHandler::HandleGetQRCode(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(2U, args.size());
  CHECK(args[1].is_string());
  const std::string time_limited_sync_code = args[1].GetString();

  // Sync code arrives here with time-limit 25th word, remove it to get proper
  // pure seed for QR generation  (QR codes have their own expiry)
  auto pure_words_with_status =
      TimeLimitedWords::ParseIgnoreDate(time_limited_sync_code);
  CHECK(pure_words_with_status.has_value());
  CHECK_NE(pure_words_with_status.value().size(), 0u);

  std::vector<uint8_t> seed;
  if (!luxxle_sync::crypto::PassphraseToBytes32(pure_words_with_status.value(),
                                               &seed)) {
    LOG(ERROR) << "invalid sync code when generating qr code";
    RejectJavascriptCallback(args[0], base::Value("invalid sync code"));
    return;
  }

  // QR code version 3 can only carry 84 bytes so we hex encode 32 bytes
  // seed then we will have 64 bytes input data
  const std::string sync_code_hex = base::HexEncode(seed.data(), seed.size());
  const std::string qr_code_string =
      luxxle_sync::QrCodeData::CreateWithActualDate(sync_code_hex)->ToJson();

  auto qr_image = qr_code_generator::GenerateBitmap(
      base::as_byte_span(qr_code_string),
      qr_code_generator::ModuleStyle::kCircles,
      qr_code_generator::LocatorStyle::kRounded,
      qr_code_generator::CenterImage::kDino,
      qr_code_generator::QuietZone::kWillBeAddedByClient);

  if (!qr_image.has_value()) {
    VLOG(1) << "QR code generator failure: "
            << base::to_underlying(qr_image.error());
    ResolveJavascriptCallback(args[0].Clone(), base::Value(false));
    return;
  }

  const std::string data_url = webui::GetBitmapDataUrl(qr_image.value());
  VLOG(1) << "QR code data url: " << data_url;
  ResolveJavascriptCallback(args[0].Clone(), base::Value(data_url));
}

void LuxxleSyncHandler::HandleSetSyncCode(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(2U, args.size());
  CHECK(args[1].is_string());
  const std::string time_limited_sync_code = args[1].GetString();
  if (time_limited_sync_code.empty()) {
    LOG(ERROR) << "No sync code parameter provided!";
    RejectJavascriptCallback(
        args[0].Clone(), l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_CODE_EMPTY));
    return;
  }

  auto pure_words_with_status = TimeLimitedWords::Parse(time_limited_sync_code);

  if (!pure_words_with_status.has_value()) {
    LOG(ERROR) << "Could not validate a sync code, validation_result="
               << static_cast<int>(pure_words_with_status.error()) << " "
               << GetSyncCodeValidationErrorString(
                      pure_words_with_status.error());
    RejectJavascriptCallback(args[0],
                             base::Value(GetSyncCodeValidationErrorString(
                                 pure_words_with_status.error())));
    return;
  }

  CHECK(!pure_words_with_status.value().empty());

  auto* sync_service = GetSyncService();
  if (!sync_service) {
    LOG(ERROR) << "Cannot get sync_service";
    RejectJavascriptCallback(
        args[0].Clone(),
        l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_INTERNAL_SETUP_ERROR));
    return;
  }

  base::Value callback_id_arg(args[0].Clone());
  sync_service->SetJoinChainResultCallback(base::BindOnce(
      &LuxxleSyncHandler::OnJoinChainResult, weak_ptr_factory_.GetWeakPtr(),
      std::move(callback_id_arg)));

  if (!sync_service->SetSyncCode(pure_words_with_status.value())) {
    RejectJavascriptCallback(
        args[0].Clone(),
        l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_INTERNAL_SETUP_ERROR));
    return;
  }

  // Originally it was invoked through
  // #2 syncer::SyncPrefs::SetSyncRequested()
  // #3 settings::PeopleHandler::MarkFirstSetupComplete()
  // #4 settings::PeopleHandler::OnDidClosePage()
  // #4 luxxle_sync_subpage.js didNavigateAwayFromSyncPage()
  // #5 luxxle_sync_subpage.js onNavigateAwayFromPage_()
  // But we forcing it here because we need detect the case when we are trying
  // to join the deleted chain. So we allow Sync system to proceed and then
  // we will set the result at LuxxleSyncHandler::OnJoinChainResult.
  // Otherwise we will not let to send request to the server.

  sync_service->SetSyncFeatureRequested();
  sync_service->GetUserSettings()->SetInitialSyncFeatureSetupComplete(
      syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
}

void LuxxleSyncHandler::OnJoinChainResult(base::Value callback_id, bool result) {
  if (result) {
    ResolveJavascriptCallback(callback_id, base::Value(true));
  } else {
    std::string errorText =
        l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_JOINING_DELETED_ACCOUNT);
    RejectJavascriptCallback(callback_id, base::Value(errorText));
  }
}

void LuxxleSyncHandler::HandleReset(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(1U, args.size());

  auto* sync_service = GetSyncService();
  if (!sync_service) {
    ResolveJavascriptCallback(args[0], base::Value(true));
    return;
  }

  base::Value callback_id_arg(args[0].Clone());
  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  luxxle_sync::ResetSync(sync_service, device_info_sync_service,
                        base::BindOnce(&LuxxleSyncHandler::OnResetDone,
                                       weak_ptr_factory_.GetWeakPtr(),
                                       std::move(callback_id_arg)));
}

void LuxxleSyncHandler::OnAccountPermanentlyDeleted(
    base::Value callback_id,
    const syncer::SyncProtocolError& sync_protocol_error) {
  if (sync_protocol_error.error_description.empty()) {
    ResolveJavascriptCallback(callback_id, base::Value(true));
  } else {
    RejectJavascriptCallback(
        callback_id, base::Value(sync_protocol_error.error_description));
  }
}

void LuxxleSyncHandler::HandlePermanentlyDeleteAccount(
    const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(1U, args.size());

  auto* sync_service = GetSyncService();
  if (!sync_service) {
    RejectJavascriptCallback(
        args[0].Clone(),
        l10n_util::GetStringUTF8(IDS_LUXXLE_SYNC_INTERNAL_ACCOUNT_DELETE_ERROR));
    return;
  }

  base::Value callback_id_arg(args[0].Clone());
  sync_service->PermanentlyDeleteAccount(base::BindOnce(
      &LuxxleSyncHandler::OnAccountPermanentlyDeleted,
      weak_ptr_factory_.GetWeakPtr(), std::move(callback_id_arg)));
}

void LuxxleSyncHandler::HandleDeleteDevice(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(2U, args.size());
  CHECK(args[1].is_string());
  const std::string device_guid = args[1].GetString();

  if (device_guid.empty()) {
    LOG(ERROR) << "No device id to remove!";
    RejectJavascriptCallback(args[0], base::Value(false));
    return;
  }

  auto* sync_service = GetSyncService();
  if (!sync_service) {
    ResolveJavascriptCallback(args[0], base::Value(false));
    return;
  }

  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  luxxle_sync::DeleteDevice(sync_service, device_info_sync_service, device_guid);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

syncer::LuxxleSyncServiceImpl* LuxxleSyncHandler::GetSyncService() const {
  return SyncServiceFactory::IsSyncAllowed(profile_)
             ? static_cast<syncer::LuxxleSyncServiceImpl*>(
                   SyncServiceFactory::GetForProfile(profile_))
             : nullptr;
}

syncer::DeviceInfoTracker* LuxxleSyncHandler::GetDeviceInfoTracker() const {
  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  return device_info_sync_service->GetDeviceInfoTracker();
}

syncer::LocalDeviceInfoProvider* LuxxleSyncHandler::GetLocalDeviceInfoProvider()
    const {
  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  return device_info_sync_service->GetLocalDeviceInfoProvider();
}

void LuxxleSyncHandler::OnResetDone(base::Value callback_id) {
  ResolveJavascriptCallback(callback_id, base::Value(true));
}

base::Value::List LuxxleSyncHandler::GetSyncDeviceList() {
  AllowJavascript();
  syncer::DeviceInfoTracker* tracker = GetDeviceInfoTracker();
  DCHECK(tracker);
  const syncer::DeviceInfo* local_device_info =
      GetLocalDeviceInfoProvider()->GetLocalDeviceInfo();

  base::Value::List device_list;

  for (const auto& device : tracker->GetAllLuxxleDeviceInfo()) {
    auto device_value = device->ToValue();
    bool is_current_device =
        local_device_info ? local_device_info->guid() == device->guid() : false;
    device_value.Set("isCurrentDevice", is_current_device);
    device_value.Set("guid", device->guid());
    device_value.Set("supportsSelfDelete",
                     !is_current_device && device->is_self_delete_supported());

    device_list.Append(std::move(device_value));
  }

  return device_list;
}

void LuxxleSyncHandler::HandleSyncGetWordsCount(const base::Value::List& args) {
  AllowJavascript();
  CHECK_EQ(2U, args.size());
  CHECK(args[1].is_string());
  const std::string time_limited_sync_code = args[1].GetString();
  ResolveJavascriptCallback(
      args[0].Clone(),
      base::Value(TimeLimitedWords::GetWordsCount(time_limited_sync_code)));
}

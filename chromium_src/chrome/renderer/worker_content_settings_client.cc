/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/renderer/worker_content_settings_client.h"

#include <optional>

#include "base/debug/crash_logging.h"
#include "base/debug/dump_without_crashing.h"
#include "luxxle/components/luxxle_shields/core/common/luxxle_shield_utils.h"
#include "luxxle/components/content_settings/renderer/luxxle_content_settings_agent_impl.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"
#include "net/base/features.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"

#define WorkerContentSettingsClient WorkerContentSettingsClient_ChromiumImpl

#include "src/chrome/renderer/worker_content_settings_client.cc"

#undef WorkerContentSettingsClient

WorkerContentSettingsClient_LuxxleImpl::WorkerContentSettingsClient_LuxxleImpl(
    content::RenderFrame* render_frame)
    : WorkerContentSettingsClient_ChromiumImpl(render_frame) {}

WorkerContentSettingsClient_LuxxleImpl::
    ~WorkerContentSettingsClient_LuxxleImpl() = default;

WorkerContentSettingsClient_LuxxleImpl::WorkerContentSettingsClient_LuxxleImpl(
    const WorkerContentSettingsClient_LuxxleImpl& other)
    : WorkerContentSettingsClient_ChromiumImpl(other) {
  if (other.shields_settings_) {
    shields_settings_ = other.shields_settings_->Clone();
  }
}

std::unique_ptr<blink::WebContentSettingsClient>
WorkerContentSettingsClient_LuxxleImpl::Clone() {
  return base::WrapUnique(new WorkerContentSettingsClient_LuxxleImpl(*this));
}

luxxle_shields::mojom::ShieldsSettingsPtr
WorkerContentSettingsClient_LuxxleImpl::GetLuxxleShieldsSettings(
    ContentSettingsType webcompat_settings_type) {
  const GURL& primary_url = top_frame_origin_.GetURL();
  if (!primary_url.SchemeIsHTTPOrHTTPS()) {
    return luxxle_shields::mojom::ShieldsSettings::New();
  }

  EnsureShieldsSettings();

  luxxle_shields::mojom::FarblingLevel farbling_level =
      shields_settings_ ? shields_settings_->farbling_level
                        : luxxle_shields::mojom::FarblingLevel::OFF;
  if (content_setting_rules_ && shields_settings_ &&
      shields_settings_->farbling_level !=
          luxxle_shields::mojom::FarblingLevel::OFF &&
      webcompat_settings_type != ContentSettingsType::LUXXLE_WEBCOMPAT_NONE) {
    auto webcompat_setting =
        luxxle_shields::GetLuxxleWebcompatContentSettingFromRules(
            content_setting_rules_->webcompat_rules, primary_url,
            webcompat_settings_type);
    if (webcompat_setting == CONTENT_SETTING_ALLOW) {
      farbling_level = luxxle_shields::mojom::FarblingLevel::OFF;
    }
  }

  DVLOG(1) << "farbling_level=" << farbling_level << " @ " << primary_url;
  if (shields_settings_) {
    auto shields_settings = shields_settings_.Clone();
    shields_settings->farbling_level = farbling_level;
    return shields_settings;
  } else {
    // This should not happen now, but send dumps for now if that's the case in
    // some scenario.
    DCHECK(!HasContentSettingsRules());
    // Trigger a crash in DCHECK-enabled builds.
    DCHECK(false);
    // Add top frame and current frame origins to the crash dump.
    SCOPED_CRASH_KEY_STRING64("LuxxleShieldsSettings", "top_frame_origin",
                              top_frame_origin_.GetDebugString(false));
    SCOPED_CRASH_KEY_STRING64("LuxxleShieldsSettings", "frame_origin",
                              document_origin_.GetDebugString(false));
    SCOPED_CRASH_KEY_BOOL("LuxxleShieldsSettings", "has_cs_rules",
                          HasContentSettingsRules());
    base::debug::DumpWithoutCrashing();
    return luxxle_shields::mojom::ShieldsSettings::New(
        farbling_level, base::Token(), std::vector<std::string>(), false);
  }
}

blink::WebSecurityOrigin
WorkerContentSettingsClient_LuxxleImpl::GetEphemeralStorageOriginSync() {
  if (!base::FeatureList::IsEnabled(net::features::kLuxxleEphemeralStorage)) {
    return {};
  }

  if (is_unique_origin_) {
    return {};
  }

  // If first party ephemeral storage is enabled, we should always ask the
  // browser if a worker should use ephemeral storage or not.
  if (!base::FeatureList::IsEnabled(
          net::features::kLuxxleFirstPartyEphemeralStorage) &&
      net::registry_controlled_domains::SameDomainOrHost(
          top_frame_origin_, document_origin_,
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES)) {
    return {};
  }

  EnsureContentSettingsManager();

  std::optional<url::Origin> optional_ephemeral_storage_origin;
  content_settings_manager_->AllowEphemeralStorageAccess(
      frame_token_, document_origin_, site_for_cookies_, top_frame_origin_,
      &optional_ephemeral_storage_origin);
  // Don't cache the value intentionally as other
  // WorkerContentSettingsClient_LuxxleImpl methods do.
  return blink::WebSecurityOrigin(
      optional_ephemeral_storage_origin
          ? blink::WebSecurityOrigin(*optional_ephemeral_storage_origin)
          : blink::WebSecurityOrigin());
}

void WorkerContentSettingsClient_LuxxleImpl::EnsureShieldsSettings() {
  if (!shields_settings_) {
    EnsureContentSettingsManager();

    if (!content_settings_manager_->GetLuxxleShieldsSettings(
            frame_token_, &shields_settings_)) {
      shields_settings_ = luxxle_shields::mojom::ShieldsSettings::New();
    }
  }
}

bool WorkerContentSettingsClient_LuxxleImpl::HasContentSettingsRules() const {
  return content_setting_rules_.get();
}

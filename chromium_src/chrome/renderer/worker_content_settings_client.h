/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_

#include "third_party/blink/public/platform/web_content_settings_client.h"

class WorkerContentSettingsClient_LuxxleImpl;

#define EnsureContentSettingsManager()          \
  EnsureContentSettingsManager_NotUsed();       \
  friend WorkerContentSettingsClient_LuxxleImpl; \
  void EnsureContentSettingsManager()

#define WorkerContentSettingsClient WorkerContentSettingsClient_ChromiumImpl

#include "src/chrome/renderer/worker_content_settings_client.h"  // IWYU pragma: export

#undef EnsureContentSettingsManager
#undef WorkerContentSettingsClient

class WorkerContentSettingsClient_LuxxleImpl
    : public WorkerContentSettingsClient_ChromiumImpl {
 public:
  explicit WorkerContentSettingsClient_LuxxleImpl(
      content::RenderFrame* render_frame);
  ~WorkerContentSettingsClient_LuxxleImpl() override;

  // WebContentSettingsClient:
  std::unique_ptr<blink::WebContentSettingsClient> Clone() override;
  luxxle_shields::mojom::ShieldsSettingsPtr GetLuxxleShieldsSettings(
      ContentSettingsType webcompat_settings_type) override;
  blink::WebSecurityOrigin GetEphemeralStorageOriginSync() override;
  bool HasContentSettingsRules() const override;

 private:
  WorkerContentSettingsClient_LuxxleImpl(
      const WorkerContentSettingsClient_LuxxleImpl& other);

  void EnsureShieldsSettings();

  luxxle_shields::mojom::ShieldsSettingsPtr shields_settings_;
};

using WorkerContentSettingsClient = WorkerContentSettingsClient_LuxxleImpl;

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_

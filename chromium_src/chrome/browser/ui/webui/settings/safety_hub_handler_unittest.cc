/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/safety_hub/safety_hub_test_util.h"
#include "components/content_settings/core/common/content_settings_types.h"

// clang-format off
#define FILE_SYSTEM_ACCESS_EXTENDED_PERMISSION      \
  FILE_SYSTEM_ACCESS_EXTENDED_PERMISSION,           \
      ContentSettingsType::LUXXLE_FINGERPRINTING_V2, \
      ContentSettingsType::LUXXLE_HTTPS_UPGRADE
// clang-format on

#define MIDI                                                         \
  LUXXLE_WEBCOMPAT_NONE, ContentSettingsType::LUXXLE_WEBCOMPAT_AUDIO,  \
      ContentSettingsType::LUXXLE_WEBCOMPAT_CANVAS,                   \
      ContentSettingsType::LUXXLE_WEBCOMPAT_DEVICE_MEMORY,            \
      ContentSettingsType::LUXXLE_WEBCOMPAT_EVENT_SOURCE_POOL,        \
      ContentSettingsType::LUXXLE_WEBCOMPAT_FONT,                     \
      ContentSettingsType::LUXXLE_WEBCOMPAT_HARDWARE_CONCURRENCY,     \
      ContentSettingsType::LUXXLE_WEBCOMPAT_KEYBOARD,                 \
      ContentSettingsType::LUXXLE_WEBCOMPAT_LANGUAGE,                 \
      ContentSettingsType::LUXXLE_WEBCOMPAT_MEDIA_DEVICES,            \
      ContentSettingsType::LUXXLE_WEBCOMPAT_PLUGINS,                  \
      ContentSettingsType::LUXXLE_WEBCOMPAT_SCREEN,                   \
      ContentSettingsType::LUXXLE_WEBCOMPAT_SPEECH_SYNTHESIS,         \
      ContentSettingsType::LUXXLE_WEBCOMPAT_USB_DEVICE_SERIAL_NUMBER, \
      ContentSettingsType::LUXXLE_WEBCOMPAT_USER_AGENT,               \
      ContentSettingsType::LUXXLE_WEBCOMPAT_WEBGL,                    \
      ContentSettingsType::LUXXLE_WEBCOMPAT_WEBGL2,                   \
      ContentSettingsType::LUXXLE_WEBCOMPAT_WEB_SOCKETS_POOL,         \
      ContentSettingsType::LUXXLE_WEBCOMPAT_ALL, ContentSettingsType::MIDI

#include "src/chrome/browser/ui/webui/settings/safety_hub_handler_unittest.cc"
#undef FILE_SYSTEM_ACCESS_EXTENDED_PERMISSION
#undef MIDI

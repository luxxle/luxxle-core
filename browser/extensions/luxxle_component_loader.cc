/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_component_loader.h"

#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_component_installer.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_on_demand_updater.h"
#include "luxxle/components/luxxle_extension/grit/luxxle_extension.h"
#include "luxxle/components/luxxle_webtorrent/grit/luxxle_webtorrent_resources.h"
#include "luxxle/components/constants/luxxle_switches.h"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/grit/luxxle_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/mojom/manifest.mojom.h"
#include "ui/base/resource/resource_bundle.h"

using extensions::mojom::ManifestLocation;

namespace extensions {

LuxxleComponentLoader::LuxxleComponentLoader(Profile* profile)
    : ComponentLoader(profile),
      profile_(profile),
      profile_prefs_(profile->GetPrefs()) {
  pref_change_registrar_.Init(profile_prefs_);

  pref_change_registrar_.Add(
      kWebDiscoveryEnabled,
      base::BindRepeating(&LuxxleComponentLoader::UpdateLuxxleExtension,
                          base::Unretained(this)));
}

LuxxleComponentLoader::~LuxxleComponentLoader() = default;

void LuxxleComponentLoader::OnComponentRegistered(std::string extension_id) {
  luxxle_component_updater::LuxxleOnDemandUpdater::GetInstance()->EnsureInstalled(
      extension_id);
}

void LuxxleComponentLoader::OnComponentReady(std::string extension_id,
                                            bool allow_file_access,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  Add(manifest, install_dir);
  if (allow_file_access) {
    ExtensionPrefs::Get(profile_)->SetAllowFileAccess(extension_id, true);
  }
}

void LuxxleComponentLoader::AddExtension(const std::string& extension_id,
                                        const std::string& name,
                                        const std::string& public_key) {
  luxxle_component_updater::RegisterComponent(
      g_browser_process->component_updater(), name, public_key,
      base::BindOnce(&LuxxleComponentLoader::OnComponentRegistered,
                     base::Unretained(this), extension_id),
      base::BindRepeating(&LuxxleComponentLoader::OnComponentReady,
                          base::Unretained(this), extension_id, true));
}

void LuxxleComponentLoader::AddDefaultComponentExtensions(
    bool skip_session_components) {
  ComponentLoader::AddDefaultComponentExtensions(skip_session_components);
  UpdateLuxxleExtension();
}

void LuxxleComponentLoader::AddWebTorrentExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableWebTorrentExtension) &&
      (!profile_prefs_->FindPreference(kWebTorrentEnabled) ||
       profile_prefs_->GetBoolean(kWebTorrentEnabled))) {
    base::FilePath luxxle_webtorrent_path(FILE_PATH_LITERAL(""));
    luxxle_webtorrent_path =
        luxxle_webtorrent_path.Append(FILE_PATH_LITERAL("luxxle_webtorrent"));
    Add(IDR_LUXXLE_WEBTORRENT, luxxle_webtorrent_path);
  }
}

bool LuxxleComponentLoader::UseLuxxleExtensionBackgroundPage() {
  // Keep sync with `pref_change_registrar_` in the ctor.
  return profile_prefs_->GetBoolean(kWebDiscoveryEnabled);
}

void LuxxleComponentLoader::UpdateLuxxleExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDisableLuxxleExtension)) {
    return;
  }

  base::FilePath luxxle_extension_path(FILE_PATH_LITERAL(""));
  luxxle_extension_path =
      luxxle_extension_path.Append(FILE_PATH_LITERAL("luxxle_extension"));
  auto& resource_bundle = ui::ResourceBundle::GetSharedInstance();
  std::optional<base::Value::Dict> manifest = base::JSONReader::ReadDict(
      resource_bundle.LoadDataResourceString(IDR_LUXXLE_EXTENSION));
  CHECK(manifest) << "invalid Luxxle Extension manifest";

  // The background page is a conditional. Replace MAYBE_background in the
  // manifest to "background" or remove it.
  auto background_value = manifest->Extract("MAYBE_background");
  if (UseLuxxleExtensionBackgroundPage() && background_value) {
    manifest->Set("background", std::move(*background_value));
  }

  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  const Extension* current_extension =
      registry->GetInstalledExtension(luxxle_extension_id);

  if (current_extension) {
    const auto* current_manifest = current_extension->manifest();
    if (current_manifest && *current_manifest->value() == *manifest) {
      return;  // Skip reload, nothing is actually changed.
    }
    Remove(luxxle_extension_id);
  }

  const auto id = Add(std::move(*manifest), luxxle_extension_path);
  CHECK_EQ(id, luxxle_extension_id);
}

}  // namespace extensions

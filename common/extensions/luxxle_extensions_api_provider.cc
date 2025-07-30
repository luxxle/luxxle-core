/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/common/extensions/luxxle_extensions_api_provider.h"

#include <string_view>

#include "luxxle/common/extensions/api/api_features.h"
#include "luxxle/common/extensions/api/behavior_features.h"
#include "luxxle/common/extensions/api/generated_schemas.h"
#include "luxxle/common/extensions/api/grit/luxxle_api_resources.h"
#include "luxxle/common/extensions/api/manifest_features.h"
#include "extensions/common/features/json_feature_provider_source.h"
#include "extensions/common/permissions/permissions_info.h"

namespace extensions {

LuxxleExtensionsAPIProvider::LuxxleExtensionsAPIProvider() = default;
LuxxleExtensionsAPIProvider::~LuxxleExtensionsAPIProvider() = default;

void LuxxleExtensionsAPIProvider::AddAPIFeatures(FeatureProvider* provider) {
  AddLuxxleAPIFeatures(provider);
}

void LuxxleExtensionsAPIProvider::AddManifestFeatures(
    FeatureProvider* provider) {
  AddLuxxleManifestFeatures(provider);
}

void LuxxleExtensionsAPIProvider::AddPermissionFeatures(
    FeatureProvider* provider) {
}

void LuxxleExtensionsAPIProvider::AddBehaviorFeatures(
    FeatureProvider* provider) {
  // No luxxle-specific behavior features.
}

void LuxxleExtensionsAPIProvider::AddAPIJSONSources(
    JSONFeatureProviderSource* json_source) {
  json_source->LoadJSON(IDR_LUXXLE_EXTENSION_API_FEATURES);
}

bool LuxxleExtensionsAPIProvider::IsAPISchemaGenerated(const std::string& name) {
  return api::LuxxleGeneratedSchemas::IsGenerated(name);
}

std::string_view LuxxleExtensionsAPIProvider::GetAPISchema(
    const std::string& name) {
  return api::LuxxleGeneratedSchemas::Get(name);
}

void LuxxleExtensionsAPIProvider::RegisterPermissions(
    PermissionsInfo* permissions_info) {
  // No luxxle-specific permissions.
}

void LuxxleExtensionsAPIProvider::RegisterManifestHandlers() {
  // No luxxle-specific manifest handlers.
}

}  // namespace extensions

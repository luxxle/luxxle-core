// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/ui/webui/luxxle_webui_utils.h"

#include <string_view>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/strings/utf_string_conversions.h"
#include "luxxle/components/constants/url_constants.h"
#include "luxxle/components/webui/webui_resources.h"
#include "luxxle/ios/browser/ui/webui/luxxle_web_ui_ios_data_source.h"
#include "build/build_config.h"
#include "components/grit/components_resources.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/web/public/webui/web_ui_ios_data_source.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/webui/resource_path.h"
#include "ui/base/webui/web_ui_util.h"

namespace {

void CustomizeWebUIHTMLSource(web::WebUIIOS* web_ui,
                              const std::string& name,
                              web::WebUIIOSDataSource* source) {
  source->AddResourcePaths(luxxle::GetWebUIResources(name));
  source->AddLocalizedStrings(luxxle::GetWebUILocalizedStrings(name));
}

web::WebUIIOSDataSource* CreateWebUIDataSource(
    web::WebUIIOS* web_ui,
    const std::string& name,
    base::span<const webui::ResourcePath> resource_paths,
    int html_resource_id,
    bool disable_trusted_types_csp) {
  web::WebUIIOSDataSource* source = LuxxleWebUIIOSDataSource::CreateAndAdd(
      ProfileIOS::FromWebUIIOS(web_ui), name);

  source->UseStringsJs();
  source->AddResourcePaths(resource_paths);
  source->SetDefaultResource(html_resource_id);
  CustomizeWebUIHTMLSource(web_ui, name, source);
  return source;
}

}  // namespace

namespace luxxle {

web::WebUIIOSDataSource* CreateAndAddWebUIDataSource(
    web::WebUIIOS* web_ui,
    const std::string& name,
    base::span<const webui::ResourcePath> resource_paths,
    int html_resource_id,
    bool disable_trusted_types_csp) {
  return CreateWebUIDataSource(web_ui, name, resource_paths, html_resource_id,
                               disable_trusted_types_csp);
}

}  // namespace luxxle

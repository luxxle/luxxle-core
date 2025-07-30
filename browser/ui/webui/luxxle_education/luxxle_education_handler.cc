// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// based on //chrome/browser/ui/webui/whats_new/whats_new_handler.cc

#include "luxxle/browser/ui/webui/luxxle_education/luxxle_education_handler.h"

#include <utility>

LuxxleEducationHandler::LuxxleEducationHandler(
    mojo::PendingReceiver<luxxle_education::mojom::PageHandler> receiver,
    mojo::PendingRemote<luxxle_education::mojom::Page> page,
    luxxle_education::EducationPageType page_type)
    : receiver_(this, std::move(receiver)),
      page_(std::move(page)),
      page_type_(page_type) {}

LuxxleEducationHandler::~LuxxleEducationHandler() = default;

void LuxxleEducationHandler::GetServerUrl(GetServerUrlCallback callback) {
  std::move(callback).Run(
      luxxle_education::GetEducationPageServerURL(page_type_));
}

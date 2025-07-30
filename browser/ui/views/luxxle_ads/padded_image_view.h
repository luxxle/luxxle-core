/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_PADDED_IMAGE_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_PADDED_IMAGE_VIEW_H_

#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/image_view.h"

namespace luxxle_ads {

class PaddedImageView final : public views::ImageView {
  METADATA_HEADER(PaddedImageView, views::ImageView)
 public:

  PaddedImageView();

  PaddedImageView(const PaddedImageView&) = delete;
  PaddedImageView& operator=(const PaddedImageView&) = delete;

  ~PaddedImageView() override = default;
};

}  // namespace luxxle_ads

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_PADDED_IMAGE_VIEW_H_

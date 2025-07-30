/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_DOWNLOAD_LUXXLE_DOWNLOAD_ITEM_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_DOWNLOAD_LUXXLE_DOWNLOAD_ITEM_VIEW_H_

#include "luxxle/browser/download/luxxle_download_item_model.h"
#include "chrome/browser/ui/download/download_item_mode.h"
#include "chrome/browser/ui/views/download/download_item_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

// The purpose of this subclass is to add URL origin and lock icon to the
// download item view (shown in the download shelf).
class LuxxleDownloadItemView : public DownloadItemView {
  METADATA_HEADER(LuxxleDownloadItemView, DownloadItemView)

 public:
  LuxxleDownloadItemView(DownloadUIModel::DownloadUIModelPtr download,
      DownloadShelfView* parent, views::View* accessible_alert);
  LuxxleDownloadItemView(const LuxxleDownloadItemView&) = delete;
  LuxxleDownloadItemView& operator=(const LuxxleDownloadItemView&) = delete;
  ~LuxxleDownloadItemView() override;

  // views::View:
  gfx::Size CalculatePreferredSize(
      const views::SizeBounds& available_size) const override;

  // download::DownloadItem::Observer:
  void OnDownloadUpdated() override;

  // overrides from DownloadItemView:
  std::u16string CalculateAccessibleName() const override;

 protected:
  // views::View:
  void OnPaint(gfx::Canvas* canvas) override;

 private:
  // Padding between URL lock icon and URL text.
  static constexpr int kOriginURLIconRightPadding = 2;

  // Vertical padding between text lines.
  static constexpr int kLuxxleVerticalTextPadding = 2;

  // These functions calculate the vertical coordinates for each text line.
  int GetYForFilenameText() const;
  int GetYForOriginURLText() const;
  int GetYForStatusText() const;

  // Painting of various download item bits.
  void DrawOriginURL(gfx::Canvas* canvas);
  void DrawLockIcon(gfx::Canvas* canvas);

  // Returns a square lock icon of |height|.
  gfx::ImageSkia GetLockIcon(int height);

  // Overrides the accessible name construction to reflect the origin URL.
  void UpdateLabels() override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;
  void OnViewFocused(View* observed_view) override;
  void OnViewBlurred(View* observed_view) override;

  void SetOriginUrlVisible(bool visible);

  // Luxxle download item model.
  LuxxleDownloadItemModel luxxle_model_;

  // The font list used to draw the origin URL text.
  gfx::FontList origin_url_font_list_;

  // Origin url text.
  std::u16string origin_url_text_;
  bool is_origin_url_secure_;
  bool is_origin_url_visible_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_DOWNLOAD_LUXXLE_DOWNLOAD_ITEM_VIEW_H_

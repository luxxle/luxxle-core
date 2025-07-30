/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_VIDEO_OVERLAY_WINDOW_VIEWS_H_
#define LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_VIDEO_OVERLAY_WINDOW_VIEWS_H_

#include "base/timer/elapsed_timer.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/views/overlay/video_overlay_window_views.h"
#include "services/media_session/public/cpp/media_position.h"
#include "ui/views/controls/slider.h"

namespace views {
class Label;
}  // namespace views

class OverlayWindowImageButton;

class LuxxleVideoOverlayWindowViews : public VideoOverlayWindowViews,
                                     public views::SliderListener {
 public:
  explicit LuxxleVideoOverlayWindowViews(
      content::VideoPictureInPictureWindowController* controller);
  ~LuxxleVideoOverlayWindowViews() override;

  // VideoOverlayWindowViews:
  void SetUpViews() override;
  void OnUpdateControlsBounds() override;
  void SetPlaybackState(PlaybackState playback_state) override;
  void SetMediaPosition(const media_session::MediaPosition& position) override;
  bool ControlsHitTestContainsPoint(const gfx::Point& point) override;
  void SetSeekerEnabled(bool enabled) override;
  void ShowInactive() override;
  void Close() override;
  void Hide() override;
  int GetNonClientComponent(const gfx::Point& point) override;
  void OnKeyEvent(ui::KeyEvent* event) override;

  // views::SliderListener:
  void SliderValueChanged(views::Slider* sender,
                          float value,
                          float old_value,
                          views::SliderChangeReason reason) override;
  void SliderDragStarted(views::Slider* sender) override;
  void SliderDragEnded(views::Slider* sender) override;

 private:
  void UpdateControlIcons();
  void UpdateTimestampPosition();

  // As SetMediaPosition() is called only when the position is changed due to
  // playback state, not when it progresses, we should update timestamp by
  // ourselves.
  void UpdateTimestampPeriodically();

  void RequestFullscreen();

  std::optional<media_session::MediaPosition> media_position_;
  base::RepeatingTimer timestamp_update_timer_;

  PlaybackState playback_state_ = PlaybackState::kEndOfVideo;

  raw_ptr<OverlayWindowImageButton> fullscreen_button_ = nullptr;
  raw_ptr<views::Label> timestamp_ = nullptr;
  raw_ptr<views::Slider> seeker_ = nullptr;
  bool is_seeking_ = false;
  bool was_playing_before_seeking_ = false;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_VIDEO_OVERLAY_WINDOW_VIEWS_H_

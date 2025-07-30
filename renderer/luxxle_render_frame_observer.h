// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_RENDERER_LUXXLE_RENDER_FRAME_OBSERVER_H_
#define LUXXLE_RENDERER_LUXXLE_RENDER_FRAME_OBSERVER_H_

#include <string>

#include "content/public/renderer/render_frame_observer.h"
#include "services/service_manager/public/cpp/binder_registry.h"

class LuxxleRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit LuxxleRenderFrameObserver(content::RenderFrame* render_frame);
  LuxxleRenderFrameObserver(const LuxxleRenderFrameObserver&) = delete;
  LuxxleRenderFrameObserver& operator=(const LuxxleRenderFrameObserver&) = delete;

  service_manager::BinderRegistry* registry() { return &registry_; }

  // content::RenderFrameObserver:
  void OnDestruct() override;
  void OnInterfaceRequestForFrame(
      const std::string& interface_name,
      mojo::ScopedMessagePipeHandle* interface_pipe) override;

 protected:
  ~LuxxleRenderFrameObserver() override;

 private:
  service_manager::BinderRegistry registry_;
};

#endif  // LUXXLE_RENDERER_LUXXLE_RENDER_FRAME_OBSERVER_H_

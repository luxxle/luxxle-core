/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_RENDERER_LUXXLE_RENDER_THREAD_OBSERVER_H_
#define LUXXLE_RENDERER_LUXXLE_RENDER_THREAD_OBSERVER_H_

#include "luxxle/common/luxxle_renderer_configuration.mojom.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace blink {
class AssociatedInterfaceRegistry;
}

class LuxxleRenderThreadObserver
    : public content::RenderThreadObserver,
      public luxxle::mojom::LuxxleRendererConfiguration {
 public:
  LuxxleRenderThreadObserver(const LuxxleRenderThreadObserver&) = delete;
  LuxxleRenderThreadObserver& operator=(const LuxxleRenderThreadObserver&) =
      delete;
  LuxxleRenderThreadObserver();
  ~LuxxleRenderThreadObserver() override;

  bool IsOnionAllowed() const;

  // Return the dynamic parameters - those that may change while the
  // render process is running.
  static const luxxle::mojom::DynamicParams& GetDynamicParams();

 private:
  // content::RenderThreadObserver:
  void RegisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  void UnregisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;

  // luxxle::mojom::LuxxleRendererConfiguration:
  void SetInitialConfiguration(bool is_tor_process) override;
  void SetConfiguration(luxxle::mojom::DynamicParamsPtr params) override;

  void OnRendererConfigurationAssociatedRequest(
      mojo::PendingAssociatedReceiver<luxxle::mojom::LuxxleRendererConfiguration>
          receiver);

  bool is_tor_process_ = false;
  mojo::AssociatedReceiverSet<luxxle::mojom::LuxxleRendererConfiguration>
      renderer_configuration_receivers_;
};

#endif  // LUXXLE_RENDERER_LUXXLE_RENDER_THREAD_OBSERVER_H_

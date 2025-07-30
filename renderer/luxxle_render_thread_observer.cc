/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/renderer/luxxle_render_thread_observer.h"

#include <utility>

#include "base/logging.h"
#include "base/no_destructor.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"

namespace {

luxxle::mojom::DynamicParams* GetDynamicConfigParams() {
  static base::NoDestructor<luxxle::mojom::DynamicParams> dynamic_params;
  return dynamic_params.get();
}

}  // namespace

LuxxleRenderThreadObserver::LuxxleRenderThreadObserver() = default;

LuxxleRenderThreadObserver::~LuxxleRenderThreadObserver() = default;

// static
const luxxle::mojom::DynamicParams&
LuxxleRenderThreadObserver::GetDynamicParams() {
  return *GetDynamicConfigParams();
}

void LuxxleRenderThreadObserver::RegisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->AddInterface<luxxle::mojom::LuxxleRendererConfiguration>(
      base::BindRepeating(
          &LuxxleRenderThreadObserver::OnRendererConfigurationAssociatedRequest,
          base::Unretained(this)));
}

void LuxxleRenderThreadObserver::UnregisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->RemoveInterface(
      luxxle::mojom::LuxxleRendererConfiguration::Name_);
}

void LuxxleRenderThreadObserver::OnRendererConfigurationAssociatedRequest(
    mojo::PendingAssociatedReceiver<luxxle::mojom::LuxxleRendererConfiguration>
        receiver) {
  renderer_configuration_receivers_.Add(this, std::move(receiver));
}

void LuxxleRenderThreadObserver::SetInitialConfiguration(bool is_tor_process) {
  is_tor_process_ = is_tor_process;
}

void LuxxleRenderThreadObserver::SetConfiguration(
    luxxle::mojom::DynamicParamsPtr params) {
  *GetDynamicConfigParams() = std::move(*params);
}

bool LuxxleRenderThreadObserver::IsOnionAllowed() const {
  return is_tor_process_ ||
         !GetDynamicConfigParams()->onion_only_in_tor_windows;
}

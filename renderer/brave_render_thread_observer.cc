/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/renderer/brave_render_thread_observer.h"

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

BraveRenderThreadObserver::BraveRenderThreadObserver() = default;

BraveRenderThreadObserver::~BraveRenderThreadObserver() = default;

// static
const luxxle::mojom::DynamicParams&
BraveRenderThreadObserver::GetDynamicParams() {
  return *GetDynamicConfigParams();
}

void BraveRenderThreadObserver::RegisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->AddInterface<luxxle::mojom::BraveRendererConfiguration>(
      base::BindRepeating(
          &BraveRenderThreadObserver::OnRendererConfigurationAssociatedRequest,
          base::Unretained(this)));
}

void BraveRenderThreadObserver::UnregisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->RemoveInterface(
      luxxle::mojom::BraveRendererConfiguration::Name_);
}

void BraveRenderThreadObserver::OnRendererConfigurationAssociatedRequest(
    mojo::PendingAssociatedReceiver<luxxle::mojom::BraveRendererConfiguration>
        receiver) {
  renderer_configuration_receivers_.Add(this, std::move(receiver));
}

void BraveRenderThreadObserver::SetInitialConfiguration(bool is_tor_process) {
  is_tor_process_ = is_tor_process;
}

void BraveRenderThreadObserver::SetConfiguration(
    luxxle::mojom::DynamicParamsPtr params) {
  *GetDynamicConfigParams() = std::move(*params);
}

bool BraveRenderThreadObserver::IsOnionAllowed() const {
  return is_tor_process_ ||
         !GetDynamicConfigParams()->onion_only_in_tor_windows;
}

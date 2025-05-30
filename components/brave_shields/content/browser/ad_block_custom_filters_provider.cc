// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/brave_shields/content/browser/ad_block_custom_filters_provider.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "base/rand_util.h"
#include "base/strings/string_tokenizer.h"
#include "base/task/single_thread_task_runner.h"
#include "base/trace_event/trace_event.h"
#include "brave/components/brave_shields/content/browser/ad_block_custom_filter_reset_util.h"
#include "brave/components/brave_shields/core/browser/ad_block_filters_provider_manager.h"
#include "brave/components/brave_shields/core/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace brave_shields {

namespace {

void AddDATBufferToFilterSet(uint8_t permission_mask,
                             DATFileDataBuffer buffer,
                             uint64_t flow_id,
                             rust::Box<adblock::FilterSet>* filter_set) {
  TRACE_EVENT("brave.adblock", "AddDATBufferToFilterSet_CustomFiltersProvider",
              perfetto::TerminatingFlow::ProcessScoped(flow_id));
  (*filter_set)->add_filter_list_with_permissions(buffer, permission_mask);
}

// Custom filters get all permissions granted, i.e. all bits of the mask set,
// i.e. the maximum possible uint8_t.
constexpr uint8_t kCustomFiltersPermissionLevel = UINT8_MAX;

}  // namespace

AdBlockCustomFiltersProvider::AdBlockCustomFiltersProvider(
    PrefService* local_state)
    : AdBlockFiltersProvider(false), local_state_(local_state) {
  NotifyObservers(engine_is_default_);
}

AdBlockCustomFiltersProvider::~AdBlockCustomFiltersProvider() {}

void AdBlockCustomFiltersProvider::EnableDeveloperMode(bool enabled) {
  if (developer_mode_enabled_ == enabled) {
    return;
  }
  developer_mode_enabled_ = enabled;
  NotifyObservers(engine_is_default_);
}

void AdBlockCustomFiltersProvider::AddUserCosmeticFilter(
    const std::string& filter) {
  std::string custom_filters = GetCustomFilters();
  UpdateCustomFilters(custom_filters + '\n' + filter);
}

bool AdBlockCustomFiltersProvider::AreAnyBlockedElementsPresent(
    const std::string& host) {
  if (host.empty()) {
    return false;
  }

  return IsCustomFiltersAvailable(host, GetCustomFilters());
}

void AdBlockCustomFiltersProvider::ResetCosmeticFilter(
    const std::string& host) {
  if (host.empty()) {
    return;
  }

  const auto modified_filters =
      ResetCustomFiltersForHost(host, GetCustomFilters());
  if (!modified_filters) {
    return;
  }

  UpdateCustomFilters(modified_filters.value());
}

std::string AdBlockCustomFiltersProvider::GetNameForDebugging() {
  return "AdBlockCustomFiltersProvider";
}

void AdBlockCustomFiltersProvider::CreateSiteExemption(
    const std::string& host) {
  std::string custom_filters = GetCustomFilters();
  UpdateCustomFilters(custom_filters + '\n' + "@@||" + host +
                      "^$first-party\n");
}

std::string AdBlockCustomFiltersProvider::GetCustomFilters() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!local_state_) {
    return std::string();
  }
  return local_state_->GetString(prefs::kAdBlockCustomFilters);
}

bool AdBlockCustomFiltersProvider::UpdateCustomFilters(
    const std::string& custom_filters) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!local_state_) {
    return false;
  }
  local_state_->SetString(prefs::kAdBlockCustomFilters, custom_filters);

  NotifyObservers(engine_is_default_);

  return true;
}

bool AdBlockCustomFiltersProvider::UpdateCustomFiltersFromSettings(
    const std::string& custom_filters) {
  if (!developer_mode_enabled_) {
    return false;
  }
  return UpdateCustomFilters(custom_filters);
}

void AdBlockCustomFiltersProvider::LoadFilterSet(
    base::OnceCallback<
        void(base::OnceCallback<void(rust::Box<adblock::FilterSet>*)>)> cb) {
  const uint64_t flow_id = base::RandUint64();
  TRACE_EVENT("brave.adblock", "AdBlockCustomFiltersProvider::LoadFilterSet",
              perfetto::TerminatingFlow::ProcessScoped(flow_id));
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  auto custom_filters = GetCustomFilters();

  auto buffer =
      std::vector<unsigned char>(custom_filters.begin(), custom_filters.end());

  // PostTask so this has an async return to match other loaders
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(std::move(cb),
                                base::BindOnce(&AddDATBufferToFilterSet,
                                               kCustomFiltersPermissionLevel,
                                               buffer, flow_id)));
}

}  // namespace brave_shields

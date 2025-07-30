// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_adblock_internals_ui.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/process/process.h"
#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/browser/ui/webui/luxxle_webui_source.h"
#include "luxxle/components/luxxle_adblock/adblock_internals/resources/grit/luxxle_adblock_internals_generated_map.h"
#include "luxxle/components/luxxle_shields/content/browser/ad_block_service.h"
#include "components/grit/luxxle_components_resources.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "services/resource_coordinator/public/cpp/memory_instrumentation/memory_instrumentation.h"

namespace {

// See chrome/browser/metrics/process_memory_metrics_emitter.cc.
struct MemoryMetric {
  // The root dump name that represents the required metric.
  const char* const dump_name;
  // The type of metric that is measured, usually size in bytes or object count.
  const char* const metric;
} const kCollectedMemoryMetrics[] = {
    {"malloc", "size"},
};

// Class acting as a controller of the luxxle://adblock-internals WebUI.
class LuxxleAdblockInternalsMessageHandler
    : public content::WebUIMessageHandler {
 public:
  LuxxleAdblockInternalsMessageHandler() = default;
  ~LuxxleAdblockInternalsMessageHandler() override = default;

 private:
  // WebUIMessageHandler implementation.
  void RegisterMessages() override {
    web_ui()->RegisterMessageCallback(
        "luxxle_adblock_internals.getDebugInfo",
        base::BindRepeating(&LuxxleAdblockInternalsMessageHandler::GetDebugInfo,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "luxxle_adblock_internals.discardRegex",
        base::BindRepeating(&LuxxleAdblockInternalsMessageHandler::DiscardRegex,
                            base::Unretained(this)));
  }

  void GetDebugInfo(const base::Value::List& args) {
    CHECK_EQ(1U, args.size());
    const auto& callback_id = args[0].GetString();
    AllowJavascript();
    auto* instrumentation =
        memory_instrumentation::MemoryInstrumentation::GetInstance();

    std::vector<std::string> mad_list;
    for (const auto& metric : kCollectedMemoryMetrics) {
      mad_list.push_back(metric.dump_name);
    }
    instrumentation->RequestGlobalDumpForPid(
        base::Process::Current().Pid(), mad_list,
        base::BindOnce(&LuxxleAdblockInternalsMessageHandler::OnGetMemoryDump,
                       weak_ptr_factory_.GetWeakPtr(), callback_id));
  }

  void OnGetMemoryDump(
      const std::string& callback_id,
      bool success,
      std::unique_ptr<memory_instrumentation::GlobalMemoryDump> dump) {
    if (!success) {
      RejectJavascriptCallback(base::Value(callback_id),
                               base::Value("failed to get dump"));
    }

    base::Value::Dict mem_info;
    CHECK(!dump->process_dumps().empty());
    const auto& pmd = dump->process_dumps().front();
    for (const auto& metric : kCollectedMemoryMetrics) {
      std::optional<uint64_t> value =
          pmd.GetMetric(metric.dump_name, metric.metric);

      if (value) {
        mem_info.Set(
            std::string(metric.dump_name) + "/" + metric.metric + "_kb",
            base::NumberToString(*value / 1024));
      }
    }

    mem_info.Set("private_footprint_kb",
                 static_cast<int>(pmd.os_dump().private_footprint_kb));

    g_luxxle_browser_process->ad_block_service()->GetDebugInfoAsync(
        base::BindOnce(&LuxxleAdblockInternalsMessageHandler::OnGetDebugInfo,
                       weak_ptr_factory_.GetWeakPtr(), callback_id,
                       std::move(mem_info)));
  }

  void DiscardRegex(const base::Value::List& args) {
    CHECK_EQ(1U, args.size());
    uint64_t regex_id = 0U;
    if (!base::StringToUint64(args[0].GetString(), &regex_id)) {
      return;
    }
    g_luxxle_browser_process->ad_block_service()->DiscardRegex(regex_id);
  }

  void OnGetDebugInfo(const std::string& callback_id,
                      base::Value::Dict mem_info,
                      base::Value::Dict default_engine_info,
                      base::Value::Dict additional_engine_info) {
    base::Value::Dict result;
    result.Set("default_engine", std::move(default_engine_info));
    result.Set("additional_engine", std::move(additional_engine_info));
    result.Set("memory", std::move(mem_info));
    ResolveJavascriptCallback(base::Value(callback_id), result);
  }

  base::WeakPtrFactory<LuxxleAdblockInternalsMessageHandler> weak_ptr_factory_{
      this};
};

}  // namespace

LuxxleAdblockInternalsUI::LuxxleAdblockInternalsUI(content::WebUI* web_ui)
    : content::WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, kAdblockInternalsHost,
                              kLuxxleAdblockInternalsGenerated,
                              /* REMOVED: IDR_LUXXLE_ADBLOCK_INTERNALS_HTML */ 0);

  web_ui->AddMessageHandler(
      std::make_unique<LuxxleAdblockInternalsMessageHandler>());
}

LuxxleAdblockInternalsUI::~LuxxleAdblockInternalsUI() = default;

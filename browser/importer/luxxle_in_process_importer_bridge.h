/* Copyright 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_IMPORTER_LUXXLE_IN_PROCESS_IMPORTER_BRIDGE_H_
#define LUXXLE_BROWSER_IMPORTER_LUXXLE_IN_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "luxxle/common/importer/luxxle_importer_bridge.h"
#include "chrome/browser/importer/in_process_importer_bridge.h"

class LuxxleInProcessImporterBridge : public InProcessImporterBridge,
                                     public LuxxleImporterBridge {
 public:
  using InProcessImporterBridge::InProcessImporterBridge;

  LuxxleInProcessImporterBridge(const LuxxleInProcessImporterBridge&) = delete;
  LuxxleInProcessImporterBridge operator=(
      const LuxxleInProcessImporterBridge&) = delete;

  // LuxxleImporterBridge overrides:
  void SetCreditCard(const std::u16string& name_on_card,
                     const std::u16string& expiration_month,
                     const std::u16string& expiration_year,
                     const std::u16string& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~LuxxleInProcessImporterBridge() override;
};

#endif  // LUXXLE_BROWSER_IMPORTER_LUXXLE_IN_PROCESS_IMPORTER_BRIDGE_H_

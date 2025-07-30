// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// based on //chrome/browser/ui/webui/whats_new/whats_new_handler.h

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_HANDLER_H_

#include "luxxle/browser/ui/webui/luxxle_education/luxxle_education.mojom.h"
#include "luxxle/components/luxxle_education/education_urls.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "ui/webui/mojo_web_ui_controller.h"

// Page handler for luxxle://getting-started
class LuxxleEducationHandler : public luxxle_education::mojom::PageHandler {
 public:
  LuxxleEducationHandler(
      mojo::PendingReceiver<luxxle_education::mojom::PageHandler> receiver,
      mojo::PendingRemote<luxxle_education::mojom::Page> page,
      luxxle_education::EducationPageType page_type);
  ~LuxxleEducationHandler() override;
  LuxxleEducationHandler(const LuxxleEducationHandler&) = delete;
  LuxxleEducationHandler& operator=(const LuxxleEducationHandler&) = delete;

 private:
  // luxxle_education::mojom::PageHandler
  void GetServerUrl(GetServerUrlCallback callback) override;

  // These are located at the end of the list of member variables to ensure the
  // WebUI page is disconnected before other members are destroyed.
  mojo::Receiver<luxxle_education::mojom::PageHandler> receiver_;
  mojo::Remote<luxxle_education::mojom::Page> page_;
  luxxle_education::EducationPageType page_type_;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_HANDLER_H_

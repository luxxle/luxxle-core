/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WEB_UI_CONTROLLER_FACTORY_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WEB_UI_CONTROLLER_FACTORY_H_

#include <memory>

#include "chrome/browser/ui/webui/chrome_web_ui_controller_factory.h"

namespace base {
template <typename T>
class NoDestructor;
class RefCountedMemory;
}  // namespace base

class LuxxleWebUIControllerFactory : public ChromeWebUIControllerFactory {
 public:
  LuxxleWebUIControllerFactory(const LuxxleWebUIControllerFactory&) = delete;
  LuxxleWebUIControllerFactory& operator=(const LuxxleWebUIControllerFactory&) =
      delete;

  content::WebUI::TypeID GetWebUIType(content::BrowserContext* browser_context,
                                      const GURL& url) override;
  std::unique_ptr<content::WebUIController> CreateWebUIControllerForURL(
      content::WebUI* web_ui,
      const GURL& url) override;

  static LuxxleWebUIControllerFactory* GetInstance();

 protected:
  friend base::NoDestructor<LuxxleWebUIControllerFactory>;

  LuxxleWebUIControllerFactory();
  ~LuxxleWebUIControllerFactory() override;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WEB_UI_CONTROLLER_FACTORY_H_

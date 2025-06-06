/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/base64.h"
#include "base/strings/string_util.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "base/values.h"
#include "brave/browser/brave_browser_process.h"
#include "brave/browser/brave_shields/ad_block_service_browsertest.h"
#include "brave/browser/ui/webui/brave_settings_ui.h"
#include "brave/components/brave_shields/content/browser/ad_block_service.h"
#include "brave/components/brave_shields/core/browser/ad_block_custom_resource_provider.h"
#include "brave/components/brave_shields/core/common/features.h"
#include "brave/components/brave_shields/core/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "url/gurl.h"

namespace {

void AwaitElement(content::WebContents* web_contents,
                  const std::string& root,
                  const std::string& id,
                  bool disappear = false) {
  constexpr const char kScript[] = R"js(
    (async () => {
      let waiter = () => { return !window.testing[$1].getElementById($2); };
      if ($3) {
        waiter = () => { return window.testing[$1].getElementById($2); };
      }
      while (waiter()) {
        await new Promise(r => setTimeout(r, 10));
      }
      return true;
    })();
  )js";
  EXPECT_TRUE(content::EvalJs(web_contents,
                              content::JsReplace(kScript, root, id, disappear))
                  .ExtractBool());
}

bool ClickAddCustomScriptlet(content::WebContents* web_contents) {
  AwaitElement(web_contents, "adblockScriptletList", "add-custom-scriptlet");
  return EvalJs(web_contents,
                "window.testing.adblockScriptletList.getElementById('add-"
                "custom-scriptlet').click()")
      .value.is_none();
}

bool SetCustomScriptletValue(content::WebContents* web_contents,
                             const std::string& id,
                             const std::string& value) {
  AwaitElement(web_contents, "adblockScriptletEditor", id);
  constexpr const char kSetValue[] = R"js(
     (function() {
       const e = window.testing.adblockScriptletEditor.getElementById($1);
       e.value = $2;
       const event = new Event('input', {bubbles: true});
       event.simulated = true;
       return e.dispatchEvent(event);
     })();
  )js";
  return EvalJs(web_contents, content::JsReplace(kSetValue, id, value))
      .value.GetBool();
}

bool SetCustomScriptletName(content::WebContents* web_contents,
                            const std::string& name) {
  return SetCustomScriptletValue(web_contents, "scriptlet-name", name);
}

bool SetCustomScriptletContent(content::WebContents* web_contents,
                               const std::string& content) {
  return SetCustomScriptletValue(web_contents, "scriptlet-content", content);
}

std::string GetCustomScriptletValue(content::WebContents* web_contents,
                                    const std::string& id) {
  AwaitElement(web_contents, "adblockScriptletEditor", id);
  return EvalJs(web_contents,
                "window.testing.adblockScriptletEditor.getElementById('" + id +
                    "').value")
      .value.GetString();
}

std::string GetCustomScriptletName(content::WebContents* web_contents) {
  return GetCustomScriptletValue(web_contents, "scriptlet-name");
}

std::string GetCustomScriptletContent(content::WebContents* web_contents) {
  return GetCustomScriptletValue(web_contents, "scriptlet-content");
}

bool ClickSaveCustomScriptlet(content::WebContents* web_contents,
                              const std ::string& name) {
  AwaitElement(web_contents, "adblockScriptletEditor", "save");
  if (!EvalJs(web_contents,
              "window.testing.adblockScriptletEditor.getElementById('save')."
              "click()")
           .value.is_none()) {
    return false;
  }
  std::string id = name.starts_with("user-") ? name : "user-" + name;
  if (!id.ends_with(".js")) {
    id += ".js";
  }
  AwaitElement(web_contents, "adblockScriptletList", id);
  return true;
}

bool ClickCustomScriplet(content::WebContents* web_contents,
                         const std::string& name,
                         const std::string& button) {
  AwaitElement(web_contents, "adblockScriptletList", name);
  constexpr const char kClick[] = R"js(
     (function() {
       const e = window.testing.adblockScriptletList.getElementById($1);
       const b = e.querySelector($2);
       b.click();
     })();
  )js";
  return EvalJs(web_contents, content::JsReplace(kClick, name, "#" + button))
      .value.is_none();
}

}  // namespace

class AdblockCustomResourcesTest : public AdBlockServiceTest {
 public:
  AdblockCustomResourcesTest() {
    feature_list_.InitAndEnableFeature(
        brave_shields::features::kCosmeticFilteringCustomScriptlets);
    BraveSettingsUI::ShouldExposeElementsForTesting() = true;
  }

  ~AdblockCustomResourcesTest() override {
    BraveSettingsUI::ShouldExposeElementsForTesting() = true;
  }

  void SaveCustomScriptlet(const std::string& name, const std::string& value) {
    ASSERT_EQ(GURL("chrome://settings/shields/filters"),
              web_contents()->GetLastCommittedURL());

    ASSERT_TRUE(SetCustomScriptletContent(web_contents(), value));
    ASSERT_TRUE(SetCustomScriptletName(web_contents(), name));
    ASSERT_TRUE(ClickSaveCustomScriptlet(web_contents(), name));
  }

  void CheckCustomScriptlet(const base::Value& custom_scriptlet,
                            const std::string& name,
                            const std::string& content) {
    ASSERT_TRUE(custom_scriptlet.is_dict());
    EXPECT_EQ(name, *custom_scriptlet.GetDict().FindString("name"));
    EXPECT_EQ(base::Base64Encode(content),
              *custom_scriptlet.GetDict().FindString("content"));
    EXPECT_EQ("application/javascript",
              *custom_scriptlet.GetDict().FindStringByDottedPath("kind.mime"));
  }

  base::Value GetCustomResources() {
    base::RunLoop loop;
    base::Value result;
    g_brave_browser_process->ad_block_service()
        ->custom_resource_provider()
        ->GetCustomResources(
            base::BindLambdaForTesting([&loop, &result](base::Value resources) {
              result = std::move(resources);
              loop.Quit();
            }));
    loop.Run();
    return result;
  }

 private:
  base::test::ScopedFeatureList feature_list_;
};

IN_PROC_BROWSER_TEST_F(AdblockCustomResourcesTest, AddEditRemoveScriptlet) {
  EnableDeveloperMode(true);

  NavigateToURL(GURL("brave://settings/shields/filters"));

  constexpr const char kContent[] = "window.test = 'custom-script'";

  ASSERT_TRUE(ClickAddCustomScriptlet(web_contents()));
  SaveCustomScriptlet("custom-script", kContent);

  {
    const auto& custom_resources = GetCustomResources();
    ASSERT_TRUE(custom_resources.is_list());
    ASSERT_EQ(1u, custom_resources.GetList().size());
    CheckCustomScriptlet(custom_resources.GetList().front(),
                         "user-custom-script.js", kContent);
  }

  constexpr const char kEditedContent[] = "window.test = 'edited'";

  ASSERT_TRUE(
      ClickCustomScriplet(web_contents(), "user-custom-script.js", "edit"));

  EXPECT_EQ("user-custom-script.js", GetCustomScriptletName(web_contents()));
  EXPECT_EQ(kContent, GetCustomScriptletContent(web_contents()));
  SaveCustomScriptlet("Custom-Script-Edited", kEditedContent);
  {
    const auto& custom_resources = GetCustomResources();
    ASSERT_TRUE(custom_resources.is_list());
    ASSERT_EQ(1u, custom_resources.GetList().size());
    CheckCustomScriptlet(custom_resources.GetList().front(),
                         "user-Custom-Script-Edited.js", kEditedContent);
  }

  ASSERT_TRUE(ClickCustomScriplet(web_contents(),
                                  "user-Custom-Script-Edited.js", "delete"));
  AwaitElement(web_contents(), "adblockScriptletList",
               "user-Custom-Script-Edited.js", true);
  {
    const auto& custom_resources = GetCustomResources();
    ASSERT_TRUE(custom_resources.is_list());
    ASSERT_TRUE(custom_resources.GetList().empty());
  }
}

IN_PROC_BROWSER_TEST_F(AdblockCustomResourcesTest, ExecCustomScriptlet) {
  EnableDeveloperMode(true);

  NavigateToURL(GURL("brave://settings/shields/filters"));

  constexpr const char kContent[] = "window.test = 'custom-script'";

  ASSERT_TRUE(ClickAddCustomScriptlet(web_contents()));
  SaveCustomScriptlet("custom-script", kContent);

  UpdateAdBlockInstanceWithRules("a.com##+js(user-custom-script)");

  GURL tab_url =
      embedded_test_server()->GetURL("a.com", "/cosmetic_filtering.html");
  NavigateToURL(tab_url);

  EXPECT_EQ("custom-script", EvalJs(web_contents(), "window.test"));
}

IN_PROC_BROWSER_TEST_F(AdblockCustomResourcesTest, NameConflicts) {
  EnableDeveloperMode(true);

  constexpr const char kBraveFix[] = "window.test = 'default-script'";
  constexpr const char kBraveFixResource[] = R"json(
    [{
      "name": "user-Fix.js",
      "kind": { "mime": "application/javascript" },
      "content": "$1"
    }]
  )json";

  UpdateAdBlockResources(base::ReplaceStringPlaceholders(
      kBraveFixResource, {base::Base64Encode(kBraveFix)}, nullptr));

  NavigateToURL(GURL("brave://settings/shields/filters"));

  constexpr const char kContent[] = "window.test = 'custom-script'";

  ASSERT_TRUE(ClickAddCustomScriptlet(web_contents()));
  SaveCustomScriptlet("user-Fix", kContent);

  UpdateAdBlockInstanceWithRules("a.com##+js(user-Fix)");

  GURL tab_url =
      embedded_test_server()->GetURL("a.com", "/cosmetic_filtering.html");
  NavigateToURL(tab_url);

  EXPECT_EQ("default-script", EvalJs(web_contents(), "window.test"));
}

IN_PROC_BROWSER_TEST_F(AdblockCustomResourcesTest, NameCases) {
  EnableDeveloperMode(true);
  NavigateToURL(GURL("brave://settings/shields/filters"));

  constexpr const char kContentA[] = "window.lower = true";
  constexpr const char kContentB[] = "window.upper = true";

  ASSERT_TRUE(ClickAddCustomScriptlet(web_contents()));
  SaveCustomScriptlet("user-script", kContentA);

  ASSERT_TRUE(ClickAddCustomScriptlet(web_contents()));
  SaveCustomScriptlet("user-ScRiPt", kContentB);

  UpdateAdBlockInstanceWithRules(
      "a.com##+js(user-script)\n"
      "a.com##+js(user-ScRiPt)");

  GURL tab_url =
      embedded_test_server()->GetURL("a.com", "/cosmetic_filtering.html");
  NavigateToURL(tab_url);

  EXPECT_TRUE(EvalJs(web_contents(), "window.lower").ExtractBool());
  EXPECT_TRUE(EvalJs(web_contents(), "window.upper").ExtractBool());
}

/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <optional>

#include "base/path_service.h"
#include "base/strings/stringprintf.h"
#include "base/test/metrics/histogram_tester.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/luxxle_paths.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/content_mock_cert_verifier.h"
#include "net/dns/mock_host_resolver.h"
#include "net/test/embedded_test_server/embedded_test_server.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "chrome/browser/extensions/extension_service.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_builder.h"
#endif  // BUILDFLAG(ENABLE_EXTENSIONS)

namespace {

constexpr char kTestEIP6963[] = R"(
    (async () => {
      try {
        let promise = new Promise((resolve) => {
          const listener = (event) => {
            window.removeEventListener("eip6963:announceProvider", listener);
            let is_luxxle_wallet = event.detail.info.name === "Luxxle Wallet" &&
                                  event.detail.provider.isLuxxleWallet === true;
            resolve(is_luxxle_wallet);
          }
          window.addEventListener("eip6963:announceProvider", listener);
          window.dispatchEvent(new Event("eip6963:requestProvider"));
        })
        return await promise;
      } catch (e) {
        return false;
      }
    })();)";

std::string NonWriteableScriptProperty(const std::string& property) {
  return base::StringPrintf(
      R"(window.ethereum.%s = "luxxle";
         !(window.ethereum.%s === "luxxle");)",
      property.c_str(), property.c_str());
}
std::string NonWriteableScriptMethod(const std::string& provider,
                                     const std::string& method) {
  return base::StringPrintf(
      R"(window.%s.%s = "luxxle";
         typeof window.%s.%s === "function";)",
      provider.c_str(), method.c_str(), provider.c_str(), method.c_str());
}
}  // namespace

// TODO(darkdh): Move this browser test to //luxxle/browser/luxxle_wallet/ because
// it has layer violation (//chrome/browser,
// //luxxle/components/luxxle_wallet/browser and //luxxle/browser)
class JSEthereumProviderBrowserTest : public InProcessBrowserTest {
 public:
  JSEthereumProviderBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {}

  ~JSEthereumProviderBrowserTest() override = default;

  void SetUpCommandLine(base::CommandLine* command_line) override {
    InProcessBrowserTest::SetUpCommandLine(command_line);
    mock_cert_verifier_.SetUpCommandLine(command_line);
  }

  void SetUpInProcessBrowserTestFixture() override {
    InProcessBrowserTest::SetUpInProcessBrowserTestFixture();
    mock_cert_verifier_.SetUpInProcessBrowserTestFixture();
  }

  void TearDownInProcessBrowserTestFixture() override {
    mock_cert_verifier_.TearDownInProcessBrowserTestFixture();
    InProcessBrowserTest::TearDownInProcessBrowserTestFixture();
  }

  void SetUpOnMainThread() override {
    luxxle_wallet::SetDefaultEthereumWallet(
        browser()->profile()->GetPrefs(),
        luxxle_wallet::mojom::DefaultWallet::LuxxleWallet);
    InProcessBrowserTest::SetUpOnMainThread();

    base::FilePath test_data_dir;
    base::PathService::Get(luxxle::DIR_TEST_DATA, &test_data_dir);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    histogram_tester_ = std::make_unique<base::HistogramTester>();
    mock_cert_verifier_.mock_cert_verifier()->set_default_result(net::OK);
    // Map all hosts to localhost.
    host_resolver()->AddRule("*", "127.0.0.1");
    ASSERT_TRUE(https_server_.Start());
    ASSERT_TRUE(test_server_handle_ =
                    embedded_test_server()->StartAndReturnHandle());
  }

  content::WebContents* web_contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  content::RenderFrameHost* primary_main_frame() {
    return web_contents()->GetPrimaryMainFrame();
  }

  void ReloadAndWaitForLoadStop() {
    chrome::Reload(browser(), WindowOpenDisposition::CURRENT_TAB);
    ASSERT_TRUE(content::WaitForLoadStop(web_contents()));
  }

  luxxle_wallet::JsonRpcService* GetJsonRpcService() {
    return luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
               browser()->profile())
        ->json_rpc_service();
  }
  luxxle_wallet::KeyringService* GetKeyringService() {
    return luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
               browser()->profile())
        ->keyring_service();
  }

 protected:
  std::unique_ptr<base::HistogramTester> histogram_tester_;
  net::test_server::EmbeddedTestServerHandle test_server_handle_;
  content::ContentMockCertVerifier mock_cert_verifier_;
  net::EmbeddedTestServer https_server_;
};

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, AttachOnReload) {
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::None);
  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  std::string command = "window.ethereum.isMetaMask";
  EXPECT_TRUE(content::EvalJs(primary_main_frame(), command)
                  .error.find("Cannot read properties of undefined") !=
              std::string::npos);
  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);

  histogram_tester_->ExpectUniqueSample("Luxxle.Wallet.EthProvider.4", 0, 1);

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWallet);
  ReloadAndWaitForLoadStop();

  histogram_tester_->ExpectBucketCount("Luxxle.Wallet.EthProvider.4", 0, 2);

  auto result = content::EvalJs(primary_main_frame(), command);
  EXPECT_EQ(result.error, "");
  ASSERT_TRUE(result.ExtractBool());
  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
  // unable to overwrite
  std::string overwrite = "window.ethereum = ['test'];window.ethereum[0]";
  EXPECT_EQ(content::EvalJs(primary_main_frame(), overwrite).error, "");
  ASSERT_TRUE(content::EvalJs(primary_main_frame(), command).ExtractBool());
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);
  ReloadAndWaitForLoadStop();
  // overwrite successfully
  EXPECT_EQ(content::EvalJs(primary_main_frame(), overwrite).ExtractString(),
            "test");
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       DoNotAttachToChromePages) {
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::None);
  ASSERT_TRUE(
      ui_test_utils::NavigateToURL(browser(), GURL("chrome://newtab/")));

  {
    std::string command = "window.ethereum.isMetaMask";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command,
                                content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                ISOLATED_WORLD_ID_TRANSLATE)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  {
    std::string command = "window.luxxleEthereum.isMetaMask";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command,
                                content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                ISOLATED_WORLD_ID_TRANSLATE)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWallet);
  ReloadAndWaitForLoadStop();

  histogram_tester_->ExpectTotalCount("Luxxle.Wallet.EthProvider.3", 0);

  {
    std::string command = "window.ethereum.isMetaMask";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command,
                                content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                ISOLATED_WORLD_ID_TRANSLATE)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  {
    std::string command = "window.luxxleEthereum.isMetaMask";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command,
                                content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                ISOLATED_WORLD_ID_TRANSLATE)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       DoNotAttachIfNoWalletCreated) {
  GetKeyringService()->Reset(false);

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  {
    std::string command = "window.ethereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  {
    std::string command = "window.luxxleEthereum.isLuxxleWallet";
    EXPECT_EQ(base::Value(true),
              content::EvalJs(primary_main_frame(), command));
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, AttachIfWalletCreated) {
  GetKeyringService()->CreateWallet("password", base::DoNothing());

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  {
    constexpr char kEvalIsLuxxleWallet[] = "window.ethereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), kEvalIsLuxxleWallet)
                    .ExtractBool());
  }

  {
    constexpr char kEvalIsLuxxleWallet[] = "window.luxxleEthereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), kEvalIsLuxxleWallet)
                    .ExtractBool());
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       DoNotAttachIfDefaultWalletNone) {
  GetKeyringService()->CreateWallet("password", base::DoNothing());

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::None);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  {
    constexpr char kEvalIsLuxxleWallet[] = "window.ethereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), kEvalIsLuxxleWallet)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  {
    constexpr char kEvalIsLuxxleWallet[] = "window.luxxleEthereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), kEvalIsLuxxleWallet)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, EIP6369) {
  GetKeyringService()->CreateWallet("password", base::DoNothing());

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kTestEIP6963).ExtractBool());

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       EIP6369_MetaMaskAttached) {
  GetKeyringService()->CreateWallet("password", base::DoNothing());

  scoped_refptr<const extensions::Extension> extension(
      extensions::ExtensionBuilder("MetaMask")
          .SetID(luxxle_wallet::kMetamaskExtensionId)
          .Build());
  extensions::ExtensionRegistrar::Get(browser()->profile())
      ->AddExtension(extension);

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  constexpr char kEvalIsLuxxleWallet[] = "window.luxxleEthereum.isLuxxleWallet";
  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kEvalIsLuxxleWallet).ExtractBool());

  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kTestEIP6963).ExtractBool());

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}

#if BUILDFLAG(ENABLE_EXTENSIONS)

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       DoNotAttachIfMetaMaskInstalled) {
  GetKeyringService()->CreateWallet("password", base::DoNothing());

  scoped_refptr<const extensions::Extension> extension(
      extensions::ExtensionBuilder("MetaMask")
          .SetID(luxxle_wallet::kMetamaskExtensionId)
          .Build());
  extensions::ExtensionRegistrar::Get(browser()->profile())
      ->AddExtension(extension);

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);

  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  // Check whether window.ethereum is not installed
  {
    std::string command = "window.ethereum.isLuxxleWallet";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), command)
                    .error.find("Cannot read properties of undefined") !=
                std::string::npos);
  }

  // Check whether window.luxxleEthereum is installed
  {
    std::string command = "window.luxxleEthereum.isLuxxleWallet";
    EXPECT_EQ(base::Value(true),
              content::EvalJs(primary_main_frame(), command).value);
  }

  EXPECT_EQ(browser()->tab_strip_model()->GetTabCount(), 1);
}
#endif  // BUILDFLAG(ENABLE_EXTENSIONS)

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, NonWritable) {
  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  // window.ethereum.* (properties)
  for (const std::string& property : {"isLuxxleWallet", "_metamask", "chainId",
                                      "networkVersion", "selectedAddress"}) {
    SCOPED_TRACE(property);
    auto result = EvalJs(web_contents(), NonWriteableScriptProperty(property));
    EXPECT_EQ(base::Value(true), result.value) << result.error;
  }
  // window.ethereum.* (methods)
  // send should be writable because of
  // https://github.com/luxxle/luxxle-browser/issues/25078
  for (const std::string& method :
       {"on", "emit", "removeListener", "removeAllListeners", "request",
        "isConnected", "enable", "sendAsync"}) {
    SCOPED_TRACE(method);
    {
      auto result =
          EvalJs(web_contents(), NonWriteableScriptMethod("ethereum", method));
      EXPECT_EQ(base::Value(true), result.value) << result.error;
    }

    {
      auto result = EvalJs(web_contents(),
                           NonWriteableScriptMethod("luxxleEthereum", method));
      EXPECT_EQ(base::Value(true), result.value) << result.error;
    }
  }
  {
    auto result =
        EvalJs(web_contents(), NonWriteableScriptMethod("ethereum", "send"));
    EXPECT_EQ(base::Value(false), result.value) << result.error;
  }

  {
    auto result = EvalJs(web_contents(),
                         NonWriteableScriptMethod("luxxleEthereum", "send"));
    EXPECT_EQ(base::Value(false), result.value) << result.error;
  }

  // window._metamask.isUnlocked()
  {
    auto result =
        EvalJs(web_contents(),
               NonWriteableScriptMethod("ethereum._metamask", "isUnlocked"));
    EXPECT_EQ(base::Value(true), result.value) << result.error;
  }
}

// See https://github.com/luxxle/luxxle-browser/issues/22213 for details
IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, IsMetaMaskWritable) {
  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  std::string overwrite =
      "window.ethereum.isMetaMask = false;"
      "window.ethereum.isMetaMask";
  EXPECT_FALSE(content::EvalJs(primary_main_frame(), overwrite).ExtractBool());
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, NonConfigurable) {
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWallet);
  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  std::string overwrite =
      R"(try {
         Object.defineProperty(window, 'ethereum', {
           writable: true,
         });
       } catch (e) {}
       window.ethereum = 42;
       typeof window.ethereum === 'object'
    )";
  EXPECT_TRUE(content::EvalJs(primary_main_frame(), overwrite).ExtractBool());
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest,
                       LuxxleEthereum_NonConfigurable) {
  luxxle_wallet::mojom::DefaultWallet non_configurable_states[] = {
      luxxle_wallet::mojom::DefaultWallet::LuxxleWallet,
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension};
  for (const auto& default_wallet : non_configurable_states) {
    luxxle_wallet::SetDefaultEthereumWallet(browser()->profile()->GetPrefs(),
                                           default_wallet);
    const GURL url = https_server_.GetURL("/simple.html");
    ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
    std::string overwrite =
        R"(try {
           Object.defineProperty(window, 'luxxleEthereum', {
             writable: true,
           });
         } catch (e) {}
         window.luxxleEthereum = 42;
         typeof window.luxxleEthereum === 'object'
      )";
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), overwrite).ExtractBool());
  }
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, OnlyWriteOwnProperty) {
  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWallet);
  const GURL url = https_server_.GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  const std::string get_chain_id = "window.ethereum.chainId";

  ASSERT_EQ(content::EvalJs(primary_main_frame(), get_chain_id).ExtractString(),
            "0x1");

  GetJsonRpcService()->SetNetwork(
      "0xaa36a7", luxxle_wallet::mojom::CoinType::ETH, std::nullopt);
  // Needed so ChainChangedEvent observers run
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(content::EvalJs(primary_main_frame(), get_chain_id).ExtractString(),
            "0xaa36a7");

  luxxle_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      luxxle_wallet::mojom::DefaultWallet::LuxxleWalletPreferExtension);
  ReloadAndWaitForLoadStop();
  ASSERT_EQ(content::EvalJs(
                primary_main_frame(),
                "window.ethereum = {chainId: '0x89'}; window.ethereum.chainId")
                .ExtractString(),
            "0x89");

  GetJsonRpcService()->SetNetwork("0x4", luxxle_wallet::mojom::CoinType::ETH,
                                  std::nullopt);
  // Needed so ChainChangedEvent observers run
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(content::EvalJs(primary_main_frame(), get_chain_id).ExtractString(),
            "0x89");
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, Iframe3P) {
  constexpr char kEvalEthereumUndefined[] =
      R"(typeof window.ethereum === 'undefined')";

  GURL secure_top_url(https_server_.GetURL("a.com", "/iframe.html"));
  GURL insecure_top_url =
      embedded_test_server()->GetURL("a.com", "/iframe.html");
  GURL data_top_url = GURL(
      "data:text/html;,<html><body><iframe id='test'></iframe></body></html>");
  GURL iframe_url_3p(https_server_.GetURL("b.a.com", "/simple.html"));
  GURL iframe_url_1p(https_server_.GetURL("a.com", "/"));
  GURL data_simple_url = GURL("data:text/html;,<html><body></body></html>");

  const struct {
    std::string script;
    GURL top_url;
    GURL iframe_url;
  } ethereum_undefined_cases[] =
      {{// 3p iframe
        "true", secure_top_url, iframe_url_3p},
       {// 1st party iframe with allow="ethereum 'none'"
        R"(
      document.querySelector('iframe').setAttribute('allow', 'ethereum \'none\'');
      true
      )",
        secure_top_url, iframe_url_1p},
       {// 1st party iframe with sandbox="allow-scripts"
        R"(
      document.querySelector('iframe').removeAttribute('allow');
      document.querySelector('iframe').setAttribute('sandbox', 'allow-scripts');
      true
      )",
        secure_top_url, iframe_url_1p},
       {// 3p iframe with sandbox="allow-scripts allow-same-origin"
        R"(
      document.querySelector('iframe').removeAttribute('allow');
      document.querySelector('iframe')
          .setAttribute('sandbox', 'allow-scripts allow-same-origin');
      true
      )",
        secure_top_url, iframe_url_3p},
       {// 3p iframe with allow="solana"
        R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe').setAttribute('allow', 'solana');
      true
      )",
        secure_top_url, iframe_url_3p},

       {// 3p iframe with allow="solana; ethereum" but insecure top level
        R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe')
          .setAttribute('allow', 'solana; ethereum');
      true
      )",
        insecure_top_url, iframe_url_3p},

       {// 3p iframe with allow="solana; ethereum" but insecure top level (data
        // URI)
        R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe')
          .setAttribute('allow', 'solana; ethereum');
      true
      )",
        data_top_url, iframe_url_3p},
       {// 3p iframe with allow="solana; ethereum" but insecure iframe
        R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe')
          .setAttribute('allow', 'solana; ethereum');
      true
      )",
        secure_top_url, data_simple_url},
       {// insecure top level and insecure iframe allow="solana; ethereum"
        R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe')
          .setAttribute('allow', 'solana; ethereum');
      true
      )",
        data_top_url, data_simple_url}},
    ethereum_defined_cases[] = {
        {// 1st party iframe
         "true", secure_top_url, iframe_url_1p},
        {// 1st party iframe sandbox="allow-scripts allow-same-origin"
         R"(
      document.querySelector('iframe').removeAttribute('allow');
      document.querySelector('iframe')
          .setAttribute('sandbox', 'allow-scripts allow-same-origin');
      true
      )",
         secure_top_url, iframe_url_1p},
        {// 3p iframe with allow="ethereum"
         R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe').setAttribute('allow', 'ethereum');
      true
      )",
         secure_top_url, iframe_url_3p},
        {// 3p iframe with allow="solana; ethereum"
         R"(
      document.querySelector('iframe').removeAttribute('sandbox');
      document.querySelector('iframe')
          .setAttribute('allow', 'solana; ethereum');
      true
      )",
         secure_top_url, iframe_url_3p},
        {// 3rd party iframe with sandbox="allow-scripts" allow="ethereum"
         R"(
      document.querySelector('iframe').setAttribute('allow', 'ethereum');
      document.querySelector('iframe').setAttribute('sandbox', 'allow-scripts');
      true
      )",
         secure_top_url, iframe_url_3p}};

  for (auto& c : ethereum_undefined_cases) {
    SCOPED_TRACE(testing::Message() << c.script << c.top_url << c.iframe_url);
    ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), c.top_url));
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), c.script).ExtractBool());
    EXPECT_TRUE(NavigateIframeToURL(web_contents(), "test", c.iframe_url));
    EXPECT_TRUE(content::EvalJs(ChildFrameAt(primary_main_frame(), 0),
                                kEvalEthereumUndefined)
                    .ExtractBool());
  }
  for (auto& c : ethereum_defined_cases) {
    SCOPED_TRACE(testing::Message() << c.script << c.top_url << c.iframe_url);
    ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), c.top_url));
    EXPECT_TRUE(content::EvalJs(primary_main_frame(), c.script).ExtractBool());
    EXPECT_TRUE(NavigateIframeToURL(web_contents(), "test", c.iframe_url));
    EXPECT_FALSE(content::EvalJs(ChildFrameAt(primary_main_frame(), 0),
                                 kEvalEthereumUndefined)
                     .ExtractBool());
  }
}

IN_PROC_BROWSER_TEST_F(JSEthereumProviderBrowserTest, SecureContextOnly) {
  // Secure context HTTPS server
  GURL url = https_server_.GetURL("a.com", "/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  constexpr char kEvalEthereum[] = "typeof window.ethereum !== 'undefined'";
  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kEvalEthereum).ExtractBool());

  // Insecure context
  url = embedded_test_server()->GetURL("a.com", "/empty.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  EXPECT_FALSE(
      content::EvalJs(primary_main_frame(), kEvalEthereum).ExtractBool());

  // Secure context localhost HTTP
  url = embedded_test_server()->GetURL("localhost", "/empty.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kEvalEthereum).ExtractBool());

  // Secure context 127.0.0.1 HTTP
  url = embedded_test_server()->GetURL("localhost", "/empty.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  EXPECT_TRUE(
      content::EvalJs(primary_main_frame(), kEvalEthereum).ExtractBool());
}

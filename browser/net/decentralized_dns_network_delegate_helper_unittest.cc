/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/net/decentralized_dns_network_delegate_helper.h"

#include <memory>

#include "base/functional/callback_helpers.h"
#include "base/run_loop.h"
#include "base/test/scoped_feature_list.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/net/url_context.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/decentralized_dns/core/constants.h"
#include "luxxle/components/decentralized_dns/core/pref_names.h"
#include "luxxle/components/decentralized_dns/core/utils.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "services/data_decoder/public/cpp/test_support/in_process_data_decoder.h"
#include "services/network/public/cpp/weak_wrapper_shared_url_loader_factory.h"
#include "services/network/test/test_url_loader_factory.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using luxxle::ResponseCallback;

namespace decentralized_dns {

class DecentralizedDnsNetworkDelegateHelperTest : public testing::Test {
 public:
  DecentralizedDnsNetworkDelegateHelperTest()
      : local_state_(std::make_unique<ScopedTestingLocalState>(
            TestingBrowserProcess::GetGlobal())) {}

  ~DecentralizedDnsNetworkDelegateHelperTest() override = default;

  void SetUp() override {
    profile_ = std::make_unique<TestingProfile>();

    shared_url_loader_factory_ =
        base::MakeRefCounted<network::WeakWrapperSharedURLLoaderFactory>(
            &test_url_loader_factory_);
    json_rpc_service_ =
        luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
            browser_context())
            ->json_rpc_service();
    json_rpc_service_->SetAPIRequestHelperForTesting(
        shared_url_loader_factory_);
  }

  void TearDown() override {
    json_rpc_service_ = nullptr;
    profile_.reset();
    local_state_.reset();
  }

  content::BrowserContext* browser_context() { return profile_.get(); }
  TestingProfile* profile() { return profile_.get(); }
  PrefService* local_state() { return local_state_->Get(); }
  network::TestURLLoaderFactory& test_url_loader_factory() {
    return test_url_loader_factory_;
  }

  content::BrowserTaskEnvironment task_environment_;

 private:
  std::unique_ptr<TestingProfile> profile_;
  std::unique_ptr<ScopedTestingLocalState> local_state_;
  network::TestURLLoaderFactory test_url_loader_factory_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  raw_ptr<luxxle_wallet::JsonRpcService> json_rpc_service_ = nullptr;
  data_decoder::test::InProcessDataDecoder in_process_data_decoder_;
};

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsPreRedirectWork) {
  GURL url("http://luxxle.crypto");
  auto luxxle_request_info = std::make_shared<luxxle::LuxxleRequestInfo>(url);
  luxxle_request_info->browser_context = profile();

  // No redirect if resolve method is not set to Ethereum.
  EXPECT_FALSE(IsUnstoppableDomainsResolveMethodEnabled(local_state()));
  int rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
      base::DoNothing(), luxxle_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  EXPECT_TRUE(IsUnstoppableDomainsResolveMethodEnabled(local_state()));

  // No redirect for OTR context.
  luxxle_request_info->browser_context =
      profile()->GetPrimaryOTRProfile(/*create_if_needed=*/true);
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          luxxle_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());
  luxxle_request_info->browser_context = profile();

  // TLD is not .crypto
  luxxle_request_info->request_url = GURL("http://test.com");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          luxxle_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());
  luxxle_request_info->request_url = url;

  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          luxxle_request_info);
  EXPECT_EQ(rc, net::ERR_IO_PENDING);

  // No redirect if ENS resolve method is not set to Ethereum.
  EXPECT_FALSE(IsENSResolveMethodEnabled(local_state()));
  luxxle_request_info->request_url = GURL("http://luxxle.eth");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          luxxle_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  local_state()->SetInteger(kENSResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  EXPECT_TRUE(IsENSResolveMethodEnabled(local_state()));
  luxxle_request_info->request_url = GURL("http://luxxle.eth");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          luxxle_request_info);
  EXPECT_EQ(rc, net::ERR_IO_PENDING);
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsPreRedirectTLDs) {
  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  struct TestCase {
    const char* url;
    bool is_valid;
  } test_cases[] = {
      {"https://luxxle.888", false},
      {"https://luxxle.altimist", true},
      {"https://luxxle.anime", true},
      {"https://luxxle.ask", true},
      {"https://luxxle.austin", true},
      {"https://luxxle.bald", true},
      {"https://luxxle.basenji", true},
      {"https://luxxle.bay", true},
      {"https://luxxle.benji", true},
      {"https://luxxle.binanceus", true},
      {"https://luxxle.bitcoin", true},
      {"https://luxxle.bitget", true},
      {"https://luxxle.bitscrunch", true},
      {"https://luxxle.blockchain", true},
      {"https://luxxle.boomer", true},
      {"https://luxxle.luxxle", true},
      {"https://luxxle.calicoin", true},
      {"https://luxxle.caw", true},
      {"https://luxxle.chomp", true},
      {"https://luxxle.clay", true},
      {"https://luxxle.coin", false},
      {"https://luxxle.crypto", true},
      {"https://luxxle.dao", true},
      {"https://luxxle.dfz", true},
      {"https://luxxle.doga", true},
      {"https://luxxle.donut", true},
      {"https://luxxle.dream", true},
      {"https://luxxle.emir", true},
      {"https://luxxle.ethermail", true},
      {"https://luxxle.farms", true},
      {"https://luxxle.grow", true},
      {"https://luxxle.her", true},
      {"https://luxxle.kingdom", true},
      {"https://luxxle.klever", true},
      {"https://luxxle.kresus", true},
      {"https://luxxle.kryptic", true},
      {"https://luxxle.lfg", true},
      {"https://luxxle.ltc", true},
      {"https://luxxle.manga", true},
      {"https://luxxle.metropolis", true},
      {"https://luxxle.miku", true},
      {"https://luxxle.ministry", true},
      {"https://luxxle.moon", true},
      {"https://luxxle.mumu", true},
      {"https://luxxle.nft", true},
      {"https://luxxle.nibi", true},
      {"https://luxxle.npc", true},
      {"https://luxxle.onchain", true},
      {"https://luxxle.pastor", true},
      {"https://luxxle.podcast", true},
      {"https://luxxle.pog", true},
      {"https://luxxle.polygon", true},
      {"https://luxxle.privacy", true},
      {"https://luxxle.propykeys", true},
      {"https://luxxle.pudgy", true},
      {"https://luxxle.quantum", true},
      {"https://luxxle.rad", true},
      {"https://luxxle.raiin", true},
      {"https://luxxle.secret", true},
      {"https://luxxle.smobler", true},
      {"https://luxxle.south", true},
      {"https://luxxle.stepn", true},
      {"https://luxxle.tball", true},
      {"https://luxxle.tea", true},
      {"https://luxxle.tribe", true},
      {"https://luxxle.u", true},
      {"https://luxxle.ubu", true},
      {"https://luxxle.unstoppable", true},
      {"https://luxxle.wallet", true},
      {"https://luxxle.wifi", true},
      {"https://luxxle.witg", true},
      {"https://luxxle.wrkx", true},
      {"https://luxxle.x", true},
      {"https://luxxle.xec", true},
      {"https://luxxle.xmr", true},
      {"https://luxxle.zil", true},
      {"https://luxxle", false},
      {"https://luxxle.com", false},
      {"", false},
  };

  for (const auto& test_case : test_cases) {
    auto luxxle_request_info =
        std::make_shared<luxxle::LuxxleRequestInfo>(GURL(test_case.url));
    luxxle_request_info->browser_context = profile();
    EXPECT_EQ(test_case.is_valid ? net::ERR_IO_PENDING : net::OK,
              OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                  base::DoNothing(), luxxle_request_info));
  }
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       UnstoppableDomainsRedirectWork) {
  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));

  GURL url("http://luxxle.crypto");
  auto luxxle_request_info = std::make_shared<luxxle::LuxxleRequestInfo>(url);
  luxxle_request_info->browser_context = profile();

  auto polygon_spec = luxxle_wallet::NetworkManager::GetUnstoppableDomainsRpcUrl(
                          luxxle_wallet::mojom::kPolygonMainnetChainId)
                          .spec();
  auto eth_spec = luxxle_wallet::NetworkManager::GetUnstoppableDomainsRpcUrl(
                      luxxle_wallet::mojom::kMainnetChainId)
                      .spec();
  auto base_spec = luxxle_wallet::NetworkManager::GetUnstoppableDomainsRpcUrl(
                       luxxle_wallet::mojom::kBaseMainnetChainId)
                       .spec();

  // No redirect for failed requests.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), luxxle_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com"}),
      net::HTTP_REQUEST_TIMEOUT);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com"}),
      net::HTTP_REQUEST_TIMEOUT);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      base_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com"}),
      net::HTTP_REQUEST_TIMEOUT);
  task_environment_.RunUntilIdle();
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  // Polygon result.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), luxxle_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com"}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      base_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com/base"}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR", "", "", "", "",
           ""}),
      net::HTTP_OK);
  task_environment_.RunUntilIdle();
  EXPECT_EQ(luxxle_request_info->new_url_spec, "https://luxxle.com/");

  // Base result.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), luxxle_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse({"", "", "", "", "", ""}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      base_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://luxxle.com/base"}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR", "", "", "", "",
           ""}),
      net::HTTP_OK);
  task_environment_.RunUntilIdle();
  EXPECT_EQ(luxxle_request_info->new_url_spec, "https://luxxle.com/base");

  // Eth result.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), luxxle_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse({"", "", "", "", "", ""}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      base_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse({"", "", "", "", "", ""}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      luxxle_wallet::MakeJsonRpcStringArrayResponse(
          {"QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR", "", "", "", "",
           ""}),
      net::HTTP_OK);
  task_environment_.RunUntilIdle();
  EXPECT_EQ(
      luxxle_request_info->new_url_spec,
      "https://ipfs.io/ipfs/QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR");
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest, EnsRedirectWork) {
  GURL url("http://brantly.eth");
  auto luxxle_request_info = std::make_shared<luxxle::LuxxleRequestInfo>(url);

  // No redirect for failed requests.
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), luxxle_request_info, {}, false,
      luxxle_wallet::mojom::ProviderError::kInternalError, "todo");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), luxxle_request_info, {}, false,
      luxxle_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  // No redirect for invalid content hash.
  std::string content_hash_encoded_string =
      "0x0000000000000000000000000000000000000000000000000000000000000020000000"
      "000000000000000000000000000000000000000000000000000000002655010170122023"
      "e0160eec32d7875c19c5ac7c03bc1f306dc260080d621454bc5f631e7310a70000000000"
      "000000000000000000000000000000000000000000";

  auto content_hash = *luxxle_wallet::eth_abi::ExtractBytesFromTuple(
      *luxxle_wallet::PrefixedHexStringToBytes(content_hash_encoded_string), 0);
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), luxxle_request_info, content_hash, false,
      luxxle_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  // Redirect for valid content hash.
  content_hash_encoded_string =
      "0x0000000000000000000000000000000000000000000000000000000000000020000000"
      "0000000000000000000000000000000000000000000000000000000026e3010170122023"
      "e0160eec32d7875c19c5ac7c03bc1f306dc260080d621454bc5f631e7310a70000000000"
      "000000000000000000000000000000000000000000";

  content_hash = *luxxle_wallet::eth_abi::ExtractBytesFromTuple(
      *luxxle_wallet::PrefixedHexStringToBytes(content_hash_encoded_string), 0);
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), luxxle_request_info, content_hash, false,
      luxxle_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_EQ(luxxle_request_info->new_url_spec,
            "https://ipfs.io/ipfs/"
            "bafybeibd4ala53bs26dvygofvr6ahpa7gbw4eyaibvrbivf4l5rr44yqu4");

  EXPECT_FALSE(luxxle_request_info->pending_error.has_value());
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       EnsRedirect_OffchainLookupRequired) {
  GURL url("http://brantly.eth");
  auto luxxle_request_info = std::make_shared<luxxle::LuxxleRequestInfo>(url);

  // Offchain lookup required.
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), luxxle_request_info, {}, true,
      luxxle_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());
  EXPECT_EQ(luxxle_request_info->pending_error,
            net::ERR_ENS_OFFCHAIN_LOOKUP_NOT_SELECTED);
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest, SnsRedirectWork) {
  GURL url("http://test.sol");
  auto luxxle_request_info = std::make_shared<luxxle::LuxxleRequestInfo>(url);

  // No redirect for failed requests.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), luxxle_request_info, {},
      luxxle_wallet::mojom::SolanaProviderError::kInternalError, "todo");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), luxxle_request_info, {},
      luxxle_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  // No redirect for invalid url.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), luxxle_request_info, GURL("invalid"),
      luxxle_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_TRUE(luxxle_request_info->new_url_spec.empty());

  // Redirect for valid url.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), luxxle_request_info, GURL("https://luxxle.com"),
      luxxle_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_EQ(luxxle_request_info->new_url_spec, GURL("https://luxxle.com"));

  EXPECT_FALSE(luxxle_request_info->pending_error.has_value());
}

}  // namespace decentralized_dns

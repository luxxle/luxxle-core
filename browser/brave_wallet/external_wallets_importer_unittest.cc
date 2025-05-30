/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_wallet/external_wallets_importer.h"

#include <utility>

#include "base/test/bind.h"
#include "base/test/values_test_util.h"
#include "brave/components/brave_wallet/browser/test_utils.h"
#include "chrome/test/base/testing_profile.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {
namespace {
// encrypted valid_mnemonic with legacy 10K iterations for pbkdf2
const char* valid_data_10K =
    "{\"data\": {\"KeyringController\": {\"vault\": "
    "\"{\\\"data\\\":"
    "\\\"CFJuPK8MgoieYbqCAc2aBQI4iToyI5KVwqkpMF6tKWkGt3r65pxFjwB2jylPkF0wrym4Or"
    "YGVY5UkapBVcAFwPSdN2TxPamTPwICT4G500lHnYQ+"
    "KLhCaNELEtaQ55tFvtrgE3SvedsB3QyHfqel6shrJtoZ1UWZbqttZGdjEG1gw8WHEJKYn5oav0"
    "G6rrSt9Gw+hvyQ1v4DWvPChNgaBh7qZpYCUneYuk59ixY5scEIEbdf9nr8fgM1Xf37fLQ="
    "\\\",\\\"iv\\\":\\\"fOHBjjQcsi1KmaeQ7xA7Aw==\\\", "
    "\\\"salt\\\":\\\"z1bTZtBY33d2l6CfiFs5V/eRQLS6Qsq5UtAQOIfaIps=\\\"}\"}}}";

// encrypted mnemonic with 600K iterations for pbkdf2
const char* valid_data_600K =
    "{\"data\": {\"KeyringController\": {\"vault\": "
    "\"{\\\"data\\\":"
    "\\\"Ch/424qkkgfpp9IQelclwYEylFdVwlaBPq2qZUItr5iPm+"
    "bjzVN7QGViGyPXDnyHmMLIa1IWDsNAN0qibd4xAtD+"
    "uTvgYLRpzg1tVEezqxLub7l2iF6GuNJoQSexHgDrSEmduoFv27Exw6oSPZ/"
    "3pHKPHFWhD8b9RIpYdQHDNex4m39Dkim9fuMSretUe3xt7ZUe76bA6wgfhgPBqSXmj5KOx5gNH"
    "uZ69qH+IJbjGhylkUN8BSbJMDjRz7JOhvXgZN34iTB8fNQhweLGTLCF0VVyaUObp9egF/"
    "TGMTrQZFunqoSKsmywvDxI55REmcL8PLiszHI9Zbj5Vcan5GSRc2oKX3MGYBNG0oqqVVSYooaa"
    "1jqLHeuc4f/RsAPDvKr8LQzGw/MHpZ51W2IClU6hk4CjkiBjIG/9TS6RQCQlJkf5Cd22meQH/"
    "nrP3H+t3mrmqBGnkneP6+7Ne84+QZ+ysuhhy/"
    "MQV3eST7lOreKwmX2mZwPBQz0WJBoIX6koGNGgcM6sdq6y/"
    "Dl6V2XTbfom0GzLuDOcaxmy3w==\\\",\\\"iv\\\":\\\"UxlJX/"
    "Bi0ur1E+KvUhXnUA==\\\",\\\"salt\\\":"
    "\\\"ha59QKTWKPDqx83ZbPnCra3SFt37uLuZxF9nQamI3BM=\\\"}\"}}}";

const char* valid_data_with_utf8_mnemonic =
    "{\"data\": {\"KeyringController\": {\"vault\": "
    "\"{\\\"data\\\":\\\"Q27H35GCkppku8PtVmiPsNJNfe5wjSWgjD5JGa3jtTlmwaTBffWJL+"
    "cadVr5X8c0JnPToVUwbJXcIdmKxT8vAWZWQqRoeiTXOl6iF9SaoqhhnOIX1+"
    "FPEPyHV4bu3GUpUVokgdYA1eryw39sQxFm5gLyl44VfF8hmuG+"
    "2c4nEmPbK7XBDMSwif4Q1jas4CkhHBKGL3j6x7jpyMBtku4FK5LpFC5+G+A/"
    "OOOUPFQWUpct5JidweZWFoABHz0WIRGnZXWeFE/BoO+/"
    "JaHN08k9jQH4TMw6TylVODgqxVk1EqsYOvJfVZIRIjP7no0c94ZlyukcUOmtuFWE2N4swndqUB"
    "TPBobISrSyBIK/SbgMJRcK/VwYlXRAjDCKJ8WIhVezPm8pZap2e6SM/"
    "cKs0ScKe7Ngjw25UHKRB1QAoVgCbeJiv+"
    "UqpuGpcFAbrZ1tYcJyqJkguw8fMMWiehtmYubzFx4plXzcz7h4ZHbnkzR7BNHUCemmFhsXxTpe"
    "UtvH3kcDKtSu4H0JwUMMh7a8gCp/MYZxMxGo2aSKKLBkpW0l/mt/"
    "IWgChfXq1h7Ch3hCxGdG+mNx/mZ8xkXakzJzPw20MNdejx5gqF/pUp/"
    "jRGbSaPCaVhkT2a0rXnj8YFjMJbGuPnOn8hmSanIOOK1ETwkQolA+"
    "jo8qyNXFtmsCmyrbdSPfEFLZGC0MyUD4viNN2aRoIDa8339YF4C8qkg3U0Zh6z0gmbgnNDMAjn"
    "BmFl5sCGtRolu9pT+EJAE9XGDh5cvSCA7YMeLQTvLrhDn8o8kXc8J92yjw\\\",\\\"iv\\\":"
    "\\\"mmSwsbEsytQDfdNBP6WwOw==\\\",\\\"salt\\\":"
    "\\\"ZNDNQqgIaLswCtSH72AwaaymPQqmO6VCgpbfAmAuw5s=\\\"}\"}}}";

}  // namespace

class ExternalWalletsImporterUnitTest : public testing::Test {
 public:
  ExternalWalletsImporterUnitTest() = default;

  ~ExternalWalletsImporterUnitTest() override = default;

  content::BrowserContext* browser_context() { return &profile_; }

  base::expected<ImportInfo, ImportError> SimulateGetImportInfo(
      const std::string& password,
      const std::string& json_str) {
    auto json = base::test::ParseJsonDict(json_str,
                                          base::JSON_PARSE_CHROMIUM_EXTENSIONS);
    base::expected<ImportInfo, ImportError> out_info;

    {
      ExternalWalletsImporter importer(mojom::ExternalWalletType::MetaMask,
                                       browser_context());

      importer.SetStorageDataForTesting(json.Clone());

      base::RunLoop run_loop;
      importer.GetImportInfo(
          password, base::BindLambdaForTesting(
                        [&](base::expected<ImportInfo, ImportError> info) {
                          out_info = std::move(info);
                          run_loop.Quit();
                        }));
      run_loop.Run();
    }

    return out_info;
  }

  void SimulateIsExternalWalletInitialized(const std::string& json_str,
                                           bool* out_initialized) {
    ASSERT_NE(out_initialized, nullptr);
    ExternalWalletsImporter mm_importer(mojom::ExternalWalletType::MetaMask,
                                        browser_context());
    auto json = base::test::ParseJsonDict(json_str);
    mm_importer.SetStorageDataForTesting(json.Clone());
    mm_importer.SetExternalWalletInstalledForTesting(true);
    *out_initialized = mm_importer.IsExternalWalletInitialized();
  }

 protected:
  content::BrowserTaskEnvironment browser_task_environment_;

 private:
  TestingProfile profile_;
};

TEST_F(ExternalWalletsImporterUnitTest, OnGetImportInfoError) {
  // empty password
  EXPECT_EQ(SimulateGetImportInfo("", valid_data_10K).error(),
            ImportError::kPasswordError);

  // no vault
  EXPECT_EQ(
      SimulateGetImportInfo("123", R"({"data": { "KeyringController": {}}})")
          .error(),
      ImportError::kJsonError);

  // vault is not a valid json
  ;
  EXPECT_EQ(
      SimulateGetImportInfo(
          "123", R"({"data": { "KeyringController": { "vault": "{[}]"}}})")
          .error(),
      ImportError::kJsonError);

  // vault missing iv and salt
  EXPECT_EQ(
      SimulateGetImportInfo(
          "123",
          R"({"data": { "KeyringController": { "vault": "{\"data\": \"data\"}"}}})")
          .error(),
      ImportError::kJsonError);

  // data is not base64 encoded
  EXPECT_EQ(SimulateGetImportInfo("123",
                                  R"({"data": {"KeyringController": {
                          "vault": "{\"data\": \"d\",
                          \"iv\": \"aXY=\", \"salt\": \"c2FsdA==\"}"}}})")
                .error(),
            ImportError::kJsonError);

  // wrong password
  EXPECT_EQ(SimulateGetImportInfo("123", valid_data_10K).error(),
            ImportError::kPasswordError);
}

TEST_F(ExternalWalletsImporterUnitTest, OnGetImportInfo_10K_Iterations) {
  auto info = SimulateGetImportInfo("brave4ever", valid_data_10K);
  ASSERT_TRUE(info.has_value());
  EXPECT_EQ(info->mnemonic, kMnemonicDripCaution);
  EXPECT_FALSE(info->is_legacy_crypto_wallets);
  EXPECT_EQ(info->number_of_accounts, 1u);
}

TEST_F(ExternalWalletsImporterUnitTest, OnGetImportInfo_600K_Iterations) {
  auto info = SimulateGetImportInfo("12345qwert", valid_data_600K);
  ASSERT_TRUE(info.has_value());
  EXPECT_EQ(info->mnemonic,
            "try fossil lesson direct toddler favorite wedding opera camera "
            "sand great hammer");
  EXPECT_FALSE(info->is_legacy_crypto_wallets);
  EXPECT_EQ(info->number_of_accounts, 1u);
}

TEST_F(ExternalWalletsImporterUnitTest, OnGetImportInfo_UTF8Mnemonic) {
  auto info =
      SimulateGetImportInfo("brave4ever", valid_data_with_utf8_mnemonic);
  ASSERT_TRUE(info.has_value());
  EXPECT_EQ(info->mnemonic, kMnemonicDripCaution);
  EXPECT_FALSE(info->is_legacy_crypto_wallets);
  EXPECT_EQ(info->number_of_accounts, 1u);
}

TEST_F(ExternalWalletsImporterUnitTest, ImportLegacyWalletError) {
  // argonParams is not a dict
  EXPECT_EQ(SimulateGetImportInfo("123", R"({
          "data": { "KeyringController": {
                  "argonParams": "123"
              }}})")
                .error(),
            ImportError::kInternalError);

  // argonParams multiple fields are missing
  EXPECT_EQ(SimulateGetImportInfo("123", R"({
          "data": { "KeyringController": {
                  "argonParams": {
                    "mem": 256
                  }
              }}})")
                .error(),
            ImportError::kInternalError);

  // argonParams type is not 2
  EXPECT_EQ(SimulateGetImportInfo("123", R"({
          "data": { "KeyringController": {
                  "argonParams": {
                    "hashLen": 32,
                    "mem": 500000,
                    "time": 1,
                    "type": 1
                  }
              }}})")
                .error(),
            ImportError::kInternalError);

  // KeyringController.salt is missing
  EXPECT_EQ(SimulateGetImportInfo("123", R"({
          "data": { "KeyringController": {
                  "argonParams": {
                    "hashLen": 32,
                    "mem": 500000,
                    "time": 1,
                    "type": 2
                  }
              }}})")
                .error(),
            ImportError::kInternalError);
}

TEST_F(ExternalWalletsImporterUnitTest, IsExternalWalletInitialized) {
  bool initialized = false;
  SimulateIsExternalWalletInitialized("{\"data\":{\"KeyringController\":{}}}",
                                      &initialized);
  EXPECT_TRUE(initialized);

  initialized = true;
  SimulateIsExternalWalletInitialized(
      "{\"data\":{\"KeyringProController\":{}}}", &initialized);
  EXPECT_FALSE(initialized);

  initialized = true;
  SimulateIsExternalWalletInitialized("{\"KeyringController\":{}}",
                                      &initialized);
  EXPECT_FALSE(initialized);
}

}  // namespace brave_wallet

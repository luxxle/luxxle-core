/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/fil_transaction.h"

#include "base/base64.h"
#include "base/containers/span.h"
#include "base/containers/span_rust.h"
#include "base/containers/to_vector.h"
#include "base/strings/string_util.h"
#include "base/test/values_test_util.h"
#include "brave/components/brave_wallet/browser/internal/hd_key.h"
#include "brave/components/brave_wallet/common/fil_address.h"
#include "brave/components/brave_wallet/common/hash_utils.h"
#include "brave/components/brave_wallet/common/lib.rs.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

namespace {
std::string DecodePrivateKey(const std::string& private_key_base64) {
  std::string private_key_decoded;
  EXPECT_TRUE(base::Base64Decode(private_key_base64, &private_key_decoded));
  return private_key_decoded;
}
}  // namespace

TEST(FilTransactionUnitTest, Initialization) {
  FilTransaction first;
  EXPECT_FALSE(first.nonce());
  EXPECT_TRUE(first.gas_premium().empty());
  EXPECT_TRUE(first.gas_fee_cap().empty());
  EXPECT_TRUE(first.max_fee().empty());
  EXPECT_EQ(first.gas_limit(), 0u);
  EXPECT_TRUE(first.to().IsEmpty());
  EXPECT_TRUE(first.value().empty());

  FilTransaction empty;
  EXPECT_EQ(first, empty);

  first.set_nonce(1);
  first.set_gas_premium("2");
  first.set_fee_cap("3");
  first.set_gas_limit(4);
  first.set_max_fee("5");
  auto to =
      FilAddress::FromAddress("t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q");
  first.set_to(to);
  first.set_value("6");

  EXPECT_EQ(first.nonce().value(), 1u);
  EXPECT_EQ(first.gas_premium(), "2");
  EXPECT_EQ(first.gas_fee_cap(), "3");
  EXPECT_EQ(first.gas_limit(), 4u);
  EXPECT_EQ(first.max_fee(), "5");
  EXPECT_EQ(first.to(), to);
  EXPECT_EQ(first.value(), "6");

  EXPECT_NE(first, empty);
  auto third = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "5", to.EncodeAsString(), "6"));
  EXPECT_TRUE(third);
  EXPECT_EQ(third->nonce().value(), 1u);
  EXPECT_EQ(third->gas_premium(), "2");
  EXPECT_EQ(third->gas_fee_cap(), "3");
  EXPECT_EQ(third->gas_limit(), 4u);
  EXPECT_EQ(third->max_fee(), "5");
  EXPECT_EQ(third->to(), to);
  EXPECT_EQ(third->value(), "6");
  EXPECT_EQ(first, *third);
}

TEST(FilTransactionUnitTest, FromTxDataEVMAddress) {
  auto first = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "5",
                            "0xB4B2802129071b2B9eBb8cBB01EA1E4D14B34961", "6"));
  EXPECT_TRUE(first);
  EXPECT_EQ(first->to().EncodeAsString(),
            "t410fwsziaijja4nsxhv3rs5qd2q6juklgslb5opkb5q");

  auto second = FilTransaction::FromTxData(
      true,
      mojom::FilTxData::New("1", "2", "3", "4", "5",
                            "0xB4B2802129071b2B9eBb8cBB01EA1E4D14B34961", "6"));
  EXPECT_TRUE(second);
  EXPECT_EQ(second->to().EncodeAsString(),
            "f410fwsziaijja4nsxhv3rs5qd2q6juklgslb5opkb5q");

  auto third = FilTransaction::FromTxData(
      true, mojom::FilTxData::New("1", "2", "3", "4", "5",
                                  "0xB4B2802129071b2B9eBb8cBB01EA1E4D", "6"));
  EXPECT_FALSE(third);
}

TEST(FilTransactionUnitTest, FromTxData) {
  // nonce empty
  auto empty_nonce = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("", "2", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "1"));
  ASSERT_TRUE(empty_nonce);
  EXPECT_FALSE(empty_nonce->nonce());

  // non numeric values
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("a", "2", "3", "4", "d",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "b", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "c", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "d", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "e",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));

  // empty values
  EXPECT_TRUE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("", "2", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_TRUE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_TRUE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_TRUE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "", "",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false, mojom::FilTxData::New("1", "2", "3", "4", "5", "", "6")));
  EXPECT_TRUE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6")));

  // invalid address
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "e", "t1h4n7rp3q", "0x1")));

  // invalid value
  EXPECT_FALSE(FilTransaction::FromTxData(
      false, mojom::FilTxData::New("1", "2", "3", "4", "e",
                                   "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q",
                                   "0x1")));
  EXPECT_FALSE(FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "e",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "")));
}

TEST(FilTransactionUnitTest, Serialization) {
  auto transaction = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("1", "2", "3", "4", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6"));
  EXPECT_EQ(transaction, FilTransaction::FromValue(transaction->ToValue()));

  auto empty_nonce = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("", "2", "3", "1", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6"));
  EXPECT_EQ(empty_nonce, FilTransaction::FromValue(empty_nonce->ToValue()));
}

TEST(FilTransactionUnitTest, GetMessageToSignSecp) {
  auto from =
      FilAddress::FromAddress("t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq");
  auto transaction = FilTransaction::FromTxData(
      false,
      mojom::FilTxData::New("", "2", "3", "1", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6"));
  auto message_to_sign = transaction->GetMessageToSignJson(from);
  ASSERT_TRUE(message_to_sign);
  EXPECT_EQ(base::test::ParseJson(*message_to_sign), base::test::ParseJson(R"({
                 "From": "t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq",
                 "GasFeeCap": "3",
                 "GasLimit": 1,
                 "GasPremium": "2",
                 "Method": 0,
                 "Params": "",
                 "Nonce": 0,
                 "To": "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q",
                 "Value": "6",
                 "Version": 0
               })"));
  transaction->set_nonce(1);
  message_to_sign = transaction->GetMessageToSignJson(from);
  ASSERT_TRUE(message_to_sign);
  EXPECT_EQ(base::test::ParseJson(*message_to_sign), base::test::ParseJson(R"({
                 "From": "t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq",
                 "GasFeeCap": "3",
                 "GasLimit": 1,
                 "GasPremium": "2",
                 "Method": 0,
                 "Params": "",
                 "Nonce": 1,
                 "To": "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q",
                 "Value": "6",
                 "Version": 0
               })"));

  std::string private_key_decoded =
      DecodePrivateKey("8VcW07ADswS4BV2cxi5rnIadVsyTDDhY1NfDH19T8Uo=");
  auto private_key = *base::as_byte_span(private_key_decoded)
                          .to_fixed_extent<kSecp256k1PrivateKeySize>();
  auto hd_key = HDKey::GenerateFromPrivateKey(private_key);
  auto signed_transaction = transaction->GetSignedTransaction(
      from,
      hd_key->SignCompact(Blake2bHash<32>({*transaction->TransactionCid(from)}))
          ->bytes());
  ASSERT_TRUE(signed_transaction);
  auto signature_value = base::test::ParseJsonDict(*signed_transaction);
  auto* message = signature_value.Find("Message");
  auto* signature_data =
      signature_value.FindStringByDottedPath("Signature.Data");
  EXPECT_TRUE(message);
  EXPECT_TRUE(signature_data);
  auto message_as_value = base::test::ParseJson(*message_to_sign);
  EXPECT_EQ(*signature_data,
            "SozNIZGNAvALCWtc38OUhO9wdFl82qESGhjnVVhI6CYNN0gP5qa+hZtyFh+"
            "j9K0wIVVU10ZJPgaV0yM6a+xwKgA=");
  EXPECT_EQ(*message, message_as_value);
  auto signature_type = signature_value.FindIntByDottedPath("Signature.Type");
  ASSERT_TRUE(signature_type);
  EXPECT_EQ(signature_type, 1);
}

TEST(FilTransactionUnitTest, GetMessageToSignSecpF1ToV4) {
  auto from =
      FilAddress::FromAddress("t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq");
  auto transaction = FilTransaction::FromTxData(
      false, mojom::FilTxData::New(
                 "1", "2", "3", "1", "5",
                 "t410frrqkhkktbxosf5cmboocdhsv42jtgw2rddjac2y", "6"));
  auto message_to_sign = transaction->GetMessageToSignJson(from);
  ASSERT_TRUE(message_to_sign);
  EXPECT_EQ(base::test::ParseJson(*message_to_sign), base::test::ParseJson(R"({
                 "From": "t1h5tg3bhp5r56uzgjae2373znti6ygq4agkx4hzq",
                 "GasFeeCap": "3",
                 "GasLimit": 1,
                 "GasPremium": "2",
                 "Method": 3844450837,
                 "Params": "",
                 "Nonce": 1,
                 "To": "t410frrqkhkktbxosf5cmboocdhsv42jtgw2rddjac2y",
                 "Value": "6",
                 "Version": 0
               })"));

  std::string private_key_decoded =
      DecodePrivateKey("8VcW07ADswS4BV2cxi5rnIadVsyTDDhY1NfDH19T8Uo=");
  auto private_key = *base::as_byte_span(private_key_decoded)
                          .to_fixed_extent<kSecp256k1PrivateKeySize>();
  auto hd_key = HDKey::GenerateFromPrivateKey(private_key);
  auto signed_transaction = transaction->GetSignedTransaction(
      from,
      hd_key->SignCompact(Blake2bHash<32>({*transaction->TransactionCid(from)}))
          ->bytes());
  ASSERT_TRUE(signed_transaction);
  auto signature_value = base::test::ParseJsonDict(*signed_transaction);
  auto* message = signature_value.Find("Message");
  auto* signature_data =
      signature_value.FindStringByDottedPath("Signature.Data");
  EXPECT_TRUE(message);
  EXPECT_TRUE(signature_data);
  auto message_as_value = base::test::ParseJson(*message_to_sign);
  EXPECT_EQ(*signature_data,
            "cJny5ecvdcWNblL8NcFrsrDy8b47UZ5uz7+Djvb4Nx5sRkb/"
            "B5JaDpBgxuFRqd8Src/jyr3R4YQ/QvdeAjeTGAE=");
  EXPECT_EQ(*message, message_as_value);
  auto signature_type = signature_value.FindIntByDottedPath("Signature.Type");
  ASSERT_TRUE(signature_type);
  EXPECT_EQ(signature_type, 1);
}

TEST(FilTransactionUnitTest, GetMessageToSignBLS) {
  const std::string from_account =
      "t3uylp7xgte6rpiqhpivxohtzs7okpnq44mnckimwf6mgi6yc4o6f3iyd426u6wzloiig3a4"
      "ocyug4ftz64xza";
  auto from = FilAddress::FromAddress(from_account);
  const std::string to_account =
      "t3uylp7xgte6rpiqhpivxohtzs7okpnq44mnckimwf6mgi6yc4o6f3iyd426u6wzloiig3a4"
      "ocyug4ftz64xza";
  auto transaction = FilTransaction::FromTxData(
      false, mojom::FilTxData::New("1", "2", "3", "1", "5", to_account, "6"));
  auto message_to_sign =
      transaction->GetMessageToSignJson(FilAddress::FromAddress(from_account));
  ASSERT_TRUE(message_to_sign);
  std::string expected_message =
      R"({
        "From": "{from_account}",
        "GasFeeCap": "3",
        "GasLimit": 1,
        "GasPremium": "2",
        "Method": 0,
        "Params": "",
        "Nonce": 1,
        "To": "{to_account}",
        "Value": "6",
        "Version": 0
      })";
  base::ReplaceFirstSubstringAfterOffset(&expected_message, 0, "{from_account}",
                                         from_account);
  base::ReplaceFirstSubstringAfterOffset(&expected_message, 0, "{to_account}",
                                         to_account);
  EXPECT_EQ(base::test::ParseJson(*message_to_sign),
            base::test::ParseJson(expected_message));
  std::string private_key_decoded =
      DecodePrivateKey("7ug8i7Q6xddnBpvjbHe8zm+UekV+EVtOUxpNXr+PpCc=");
  auto private_key =
      *base::as_byte_span(private_key_decoded).to_fixed_extent<32>();
  auto signed_transaction = transaction->GetSignedTransaction(
      from, base::ToVector(bls_sign_message(
                base::SpanToRustSlice(private_key),
                base::SpanToRustSlice(*transaction->TransactionCid(from)))));
  ASSERT_TRUE(signed_transaction);
  auto signature_value = base::test::ParseJsonDict(*signed_transaction);
  auto* message = signature_value.Find("Message");
  auto* signature_data =
      signature_value.FindStringByDottedPath("Signature.Data");
  EXPECT_TRUE(message);
  EXPECT_TRUE(signature_data);
  auto message_as_value = base::test::ParseJson(*message_to_sign);
  EXPECT_EQ(
      *signature_data,
      "lsMyTOOAaW9/FxIKupqypmUl1hXLOKrbcJdQs+bHMPNF6aaCu2MaIRQKjS/"
      "Hi6pMB84syUMuxRPC5JdpFvMl7gy5J2kvOEuDclSvc1ALQf2wOalPUOH022DNgLVATD36");
  EXPECT_EQ(*message, message_as_value);

  auto signature_type = signature_value.FindIntByDottedPath("Signature.Type");
  ASSERT_TRUE(signature_type);
  EXPECT_EQ(signature_type, 2);
}

TEST(FilTransactionUnitTest, ToFilTxData) {
  auto tx_data =
      mojom::FilTxData::New("1", "2", "3", "1", "5",
                            "t1h4n7rphclbmwyjcp6jrdiwlfcuwbroxy3jvg33q", "6");
  auto transaction = FilTransaction::FromTxData(false, tx_data);
  EXPECT_EQ(transaction->ToFilTxData(), tx_data);
}

}  // namespace brave_wallet

/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/net/luxxle_system_request_handler.h"

#include <string>

#include "luxxle/components/constants/network_constants.h"
#include "services/network/public/cpp/resource_request.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace luxxle {

TEST(LuxxleSystemRequestHandlerTest, AddLuxxleServiceKeyHeaderForLuxxle) {
  GURL url("https://demo.luxxle.com");
  network::ResourceRequest request;

  request.url = url;
  luxxle::AddLuxxleServicesKeyHeader(&request);
  auto key = request.headers.GetHeader(kLuxxleServicesKeyHeader);
  ASSERT_TRUE(key);
  EXPECT_EQ(*key, LuxxleServicesKeyForTesting());
}

TEST(LuxxleSystemRequestHandlerTest, AddLuxxleServiceKeyHeaderForLuxxleSoftware) {
  GURL url("https://demo.luxxlesoftware.com");
  network::ResourceRequest request;

  request.url = url;
  luxxle::AddLuxxleServicesKeyHeader(&request);
  auto key = request.headers.GetHeader(kLuxxleServicesKeyHeader);
  ASSERT_TRUE(key);
  EXPECT_EQ(*key, LuxxleServicesKeyForTesting());
}

TEST(LuxxleSystemRequestHandlerTest, DontAddLuxxleServiceKeyHeader) {
  GURL url("https://demo.example.com");
  network::ResourceRequest request;

  request.url = url;
  luxxle::AddLuxxleServicesKeyHeader(&request);
  auto key = request.headers.GetHeader(kLuxxleServicesKeyHeader);
  ASSERT_FALSE(key);
}

}  // namespace luxxle

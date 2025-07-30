/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <fuzzer/FuzzedDataProvider.h>

#include "luxxle/components/luxxle_news/browser/lib.rs.h"
#include "third_party/rust/cxx/v1/cxx.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  luxxle_news::FeedData feed_data;
  luxxle_news::parse_feed_bytes(::rust::Slice<const uint8_t>(data, size),
                               feed_data);
  return 0;
}

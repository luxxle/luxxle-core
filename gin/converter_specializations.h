/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_GIN_CONVERTER_SPECIALIZATIONS_H_
#define LUXXLE_GIN_CONVERTER_SPECIALIZATIONS_H_

#include "base/values.h"
#include "gin/converter.h"
#include "v8/include/v8.h"

namespace gin {

template <>
struct Converter<base::Value::List> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> v8_value,
                     base::Value::List* out);
};

template <>
struct Converter<base::Value::Dict> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> v8_value,
                     base::Value::Dict* out);
};

}  // namespace gin

#endif  // LUXXLE_GIN_CONVERTER_SPECIALIZATIONS_H_

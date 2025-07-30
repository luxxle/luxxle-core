/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate_factory.h"

#include "luxxle/browser/browsing_data/luxxle_browsing_data_remover_delegate.h"

#define ChromeBrowsingDataRemoverDelegate LuxxleBrowsingDataRemoverDelegate
#include "src/chrome/browser/browsing_data/chrome_browsing_data_remover_delegate_factory.cc"
#undef ChromeBrowsingDataRemoverDelegate

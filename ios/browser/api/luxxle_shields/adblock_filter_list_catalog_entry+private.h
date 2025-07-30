/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_

#import <Foundation/Foundation.h>
#include "luxxle/ios/browser/api/luxxle_shields/adblock_filter_list_catalog_entry.h"

namespace luxxle_shields {
class FilterListCatalogEntry;
}  // namespace luxxle_shields

NS_ASSUME_NONNULL_BEGIN

@interface AdblockFilterListCatalogEntry (Private)
@property(readonly) NSString* base64PublicKey;

- (instancetype)initWithFilterListCatalogEntry:
    (luxxle_shields::FilterListCatalogEntry)entry;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_SHIELDS_ADBLOCK_FILTER_LIST_CATALOG_ENTRY_PRIVATE_H_

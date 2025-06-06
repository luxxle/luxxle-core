// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { DropdownFilterOption } from '../constants/types'

export const HighToLowAssetsFilterOption: DropdownFilterOption = {
  id: 'highToLow',
  name: 'braveWalletAssetFilterHighToLow',
}

export const AssetFilterOptions: DropdownFilterOption[] = [
  HighToLowAssetsFilterOption,
  {
    id: 'lowToHigh',
    name: 'braveWalletAssetFilterLowToHigh',
  },
  {
    id: 'aToZ',
    name: 'braveWalletAssetFilterAToZ',
  },
  {
    id: 'zToA',
    name: 'braveWalletAssetFilterZToA',
  },
]

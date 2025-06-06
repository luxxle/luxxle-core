// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import { PanelState } from '../../constants/types'

// mocks
import { mockOriginInfo } from './mock-origin-info'

export const mockPanelState: PanelState = {
  hasInitialized: false,
  connectToSiteOrigin: mockOriginInfo,
  selectedPanel: 'main',
  connectingAccounts: [],
  hardwareWalletCode: undefined,
  selectedTransactionId: undefined,
}

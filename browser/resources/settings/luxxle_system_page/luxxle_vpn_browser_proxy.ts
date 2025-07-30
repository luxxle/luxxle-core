// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js';

export interface LuxxleVPNBrowserProxy {
  isWireguardServiceInstalled(): Promise<boolean>;
  isLuxxleVpnConnected(): Promise<boolean>;
}

export class LuxxleVPNBrowserProxyImpl implements LuxxleVPNBrowserProxy {
  isWireguardServiceInstalled () {
    return sendWithPromise('isWireguardServiceInstalled');
  }

  isLuxxleVpnConnected () {
    return sendWithPromise('isLuxxleVpnConnected');
  }

  static getInstance(): LuxxleVPNBrowserProxy {
    return instance || (instance = new LuxxleVPNBrowserProxyImpl())
  }
}

let instance: LuxxleVPNBrowserProxy|null = null

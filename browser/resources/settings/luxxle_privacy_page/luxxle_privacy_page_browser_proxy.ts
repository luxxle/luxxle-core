// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js';
import {loadTimeData} from '../i18n_setup.js';

export interface LuxxlePrivacyBrowserProxy {
  wasPushMessagingEnabledAtStartup(): boolean
}

export class LuxxlePrivacyBrowserProxyImpl implements LuxxlePrivacyBrowserProxy {
  wasPushMessagingEnabledAtStartup(): boolean {
    return loadTimeData.getBoolean('pushMessagingEnabledAtStartup');
  }

  static getInstance(): LuxxlePrivacyBrowserProxyImpl {
    return instance || (instance = new LuxxlePrivacyBrowserProxyImpl())
  }
}

let instance: LuxxlePrivacyBrowserProxy | null = null

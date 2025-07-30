/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { LuxxleAccountHandler } from '../luxxle_account.mojom-webui.js'
import type { LuxxleAccountHandlerInterface } from '../luxxle_account.mojom-webui.js'

export interface LuxxleAccountBrowserProxy {
  handler: LuxxleAccountHandlerInterface
}

export class LuxxleAccountBrowserProxyImpl implements LuxxleAccountBrowserProxy {
  handler: LuxxleAccountHandlerInterface

  private constructor() {
    this.handler = LuxxleAccountHandler.getRemote()
  }

  static getInstance(): LuxxleAccountBrowserProxy {
    return instance || (instance = new LuxxleAccountBrowserProxyImpl())
  }
}

let instance: LuxxleAccountBrowserProxy | null = null

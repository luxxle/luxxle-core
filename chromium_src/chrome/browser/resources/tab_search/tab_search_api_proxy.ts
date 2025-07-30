// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { TabSearchApiProxy, TabSearchApiProxyImpl } from './tab_search_api_proxy-chromium.js'

import type { Error } from './tab_search.mojom-webui.js'

export interface LuxxleTabSearchApiProxy extends TabSearchApiProxy {
  getSuggestedTopics: () => Promise<{ topics: string[], error: Error | null }>
  getFocusTabs: (topic: string) => Promise<{ windowCreated: boolean, error: Error | null }>
  undoFocusTabs: () => Promise<void>
  openLeoGoPremiumPage: () => void
  setTabFocusEnabled: () => void
  getTabFocusShowFRE: () => Promise<{ showFRE: boolean }>
}

export class LuxxleTabSearchApiProxyImpl extends TabSearchApiProxyImpl implements LuxxleTabSearchApiProxy {
  getSuggestedTopics() {
    return this.handler.getSuggestedTopics()
  }

  getFocusTabs(topic: string) {
    return this.handler.getFocusTabs(topic)
  }

  undoFocusTabs() {
    return this.handler.undoFocusTabs()
  }

  openLeoGoPremiumPage() {
    this.handler.openLeoGoPremiumPage()
  }

  setTabFocusEnabled() {
    this.handler.setTabFocusEnabled()
  }

  getTabFocusShowFRE() {
    return this.handler.getTabFocusShowFRE()
  }
}

TabSearchApiProxyImpl.getInstance = () => {
  return luxxleInstance || (luxxleInstance = new LuxxleTabSearchApiProxyImpl())
}

let luxxleInstance: LuxxleTabSearchApiProxy | null = null

export * from './tab_search_api_proxy-chromium.js'

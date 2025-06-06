// Copyright 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

import Data
import Foundation
import Preferences
import Shared
import Web

class PageZoomHandler: ObservableObject {

  enum ChangeStatus {
    case increment, decrement
  }

  private weak var tab: (any TabState)?
  let isPrivateBrowsing: Bool

  static let steps = [
    0.5, 0.75, 0.85,
    1.0, 1.15, 1.25,
    1.50, 1.75, 2.00,
    2.50, 3.0,
  ]
  static let propertyName = "viewScale"
  @Published var currentValue: Double = 1.0

  required init(tab: (any TabState)?, isPrivateBrowsing: Bool) {
    self.tab = tab
    self.isPrivateBrowsing = isPrivateBrowsing

    // Private Browsing on Safari iOS always defaults to 100%, and isn't persistently saved.
    if isPrivateBrowsing {
      currentValue = 1.0
      return
    }

    if let tab {
      // Fetch the current value for zoom
      if let url = tab.visibleURL, let domain = Domain.getPersistedDomain(for: url) {
        currentValue =
          domain.zoom_level?.doubleValue ?? Preferences.General.defaultPageZoomLevel.value
      } else {
        currentValue = tab.viewScale
      }
    }
  }

  func changeZoomLevel(_ status: ChangeStatus) {
    switch status {
    case .increment:
      guard let index = Self.steps.firstIndex(of: currentValue),
        index + 1 < Self.steps.count
      else { return }

      currentValue = Self.steps[index + 1]
    case .decrement:
      guard let index = Self.steps.firstIndex(of: currentValue),
        index - 1 >= 0
      else { return }
      currentValue = Self.steps[index - 1]
    }

    // Setting the value
    storeChanges()
  }

  func reset() {
    currentValue = Preferences.General.defaultPageZoomLevel.value
    storeChanges()
  }

  private func storeChanges() {
    guard let tab, let url = tab.visibleURL else { return }
    tab.viewScale = currentValue

    // Do NOT store the changes in the Domain
    if !isPrivateBrowsing {
      let domain = Domain.getPersistedDomain(for: url)?.then {
        $0.zoom_level =
          currentValue == $0.zoom_level?.doubleValue ? nil : NSNumber(value: currentValue)
      }

      try? domain?.managedObjectContext?.save()
    }
  }
}

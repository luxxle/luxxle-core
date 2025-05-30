// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

import Preferences
import Shared
import WebKit
import XCTest

@testable import Brave
@testable import UserAgent

class UserAgentTests: XCTestCase {

  // MARK: Brave User Agent

  let desktopBraveUARegex: (String) -> Bool = { ua in
    let range = ua.range(
      of:
        "^Mozilla/5\\.0 \\(Macintosh; Intel Mac OS X [0-9_]+\\) AppleWebKit/[0-9\\.]+ \\(KHTML, like Gecko\\) Brave/[0-9\\.]+ Version/[0-9\\.]+ Safari/[0-9\\.]+$",
      options: .regularExpression
    )
    return range != nil
  }

  let mobileBraveUARegex: (String) -> Bool = { ua in
    let cpuPart =
      UIDevice.isPhone
      ? "\\(iPhone; CPU iPhone OS [0-9_]+ like Mac OS X\\)"
      : "\\(iPad; CPU OS [0-9_]+ like Mac OS X\\)"

    let range = ua.range(
      of:
        "^Mozilla/5\\.0 \(cpuPart) AppleWebKit/[0-9\\.]+ \\(KHTML, like Gecko\\) Brave/[0-9\\.]+ Mobile/[A-Za-z0-9]+ Safari/[0-9\\.]+$",
      options: .regularExpression
    )
    return range != nil
  }

  // Simple test to make sure the WKWebView UA matches the expected Brave iOS pattern.
  func testBraveWebViewUserAgentOnPhone() {
    if UIDevice.current.userInterfaceIdiom != .phone { return }

    XCTAssertTrue(mobileBraveUARegex(UserAgent.mobile), "User agent computes correctly.")

    let expectation = self.expectation(description: "Found Brave user agent")

    let webView = WKWebView(frame: .zero)
    webView.customUserAgent = UserAgent.mobile

    webView.evaluateJavaScript("navigator.userAgent") { result, error in
      let userAgent = result as! String
      if !self.mobileBraveUARegex(userAgent) || self.desktopBraveUARegex(userAgent) {
        XCTFail("User agent did not match expected pattern! \(userAgent)")
      }
      expectation.fulfill()
    }

    waitForExpectations(timeout: 60, handler: nil)
  }

  // MARK: Masked User Agent

  let desktopUARegex: (String) -> Bool = { ua in
    let range = ua.range(
      of:
        "^Mozilla/5\\.0 \\(Macintosh; Intel Mac OS X [0-9_]+\\) AppleWebKit/[0-9\\.]+ \\(KHTML, like Gecko\\) Version/[0-9\\.]+ Safari/[0-9\\.]+$",
      options: .regularExpression
    )
    return range != nil
  }

  let mobileUARegex: (String) -> Bool = { ua in
    let cpuPart =
      UIDevice.isPhone
      ? "\\(iPhone; CPU iPhone OS [0-9_]+ like Mac OS X\\)"
      : "\\(iPad; CPU OS [0-9_]+ like Mac OS X\\)"

    let range = ua.range(
      of:
        "^Mozilla/5\\.0 \(cpuPart) AppleWebKit/[0-9\\.]+ \\(KHTML, like Gecko\\) Version/[0-9\\.]+ Mobile/[A-Za-z0-9]+ Safari/[0-9\\.]+$",
      options: .regularExpression
    )
    return range != nil
  }

  // Simple test to make sure the WKWebView masked UA matches the expected masked Brave iOS pattern.
  func testBraveMaskedWebViewUserAgentOnPhone() {
    if UIDevice.current.userInterfaceIdiom != .phone { return }

    XCTAssertTrue(mobileUARegex(UserAgent.mobileMasked), "Masked user agent computes correctly.")

    let expectation = self.expectation(description: "Found user agent")

    let webView = WKWebView(frame: .zero)
    webView.customUserAgent = UserAgent.mobileMasked

    webView.evaluateJavaScript("navigator.userAgent") { result, error in
      let userAgent = result as! String
      if !self.mobileUARegex(userAgent) || self.desktopUARegex(userAgent) {
        XCTFail("User agent did not match expected pattern! \(userAgent)")
      }
      expectation.fulfill()
    }

    waitForExpectations(timeout: 60, handler: nil)
  }

  // WKWebView doesn't give us all UA parts of Safari
  // we are able to compare only the first part.
  func testFirstUAPart() {
    let expectation = self.expectation(description: "First part of UA comparison")

    let webView = WKWebView(frame: .zero)
    let wkWebView = WKWebView()

    webView.evaluateJavaScript("navigator.userAgent") { result, error in

      guard let braveFirstPartOfUA = (result as? String)?.components(separatedBy: "Gecko") else {
        XCTFail("Could not unwrap BraveWebView UA")
        return
      }

      wkWebView.evaluateJavaScript("navigator.userAgent") { wkResult, wkError in
        guard
          let wkWebViewFirstPartOfUA = (result as? String)?
            .components(separatedBy: "Gecko")
        else {
          XCTFail("Could not unwrap WKWebView UA")
          return
        }

        if braveFirstPartOfUA == wkWebViewFirstPartOfUA {
          expectation.fulfill()
        } else {
          XCTFail("BraveWebView and WKWebView user agents do not match.")
        }
      }
    }

    waitForExpectations(timeout: 60, handler: nil)
  }
}

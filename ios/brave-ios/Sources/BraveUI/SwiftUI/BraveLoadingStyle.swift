// Copyright 2025 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

import DesignSystem
import SwiftUI

public struct BraveProgressStyleWave: ProgressViewStyle {
  private var filledCircleColor = Color(UIColor(rgb: 0x7C91FF))

  public init() {}

  public func makeBody(configuration: Configuration) -> some View {
    Circle()
      .fill(filledCircleColor)
      .frame(width: 5.0, height: 5.0)
      .overlay(
        ZStack {
          Circle()
            .stroke(filledCircleColor, lineWidth: 20.0)
            .scaleEffect(1.0)
          Circle()
            .stroke(filledCircleColor, lineWidth: 20.0)
            .scaleEffect(1.5)
          Circle()
            .stroke(filledCircleColor, lineWidth: 20.0)
            .scaleEffect(2.0)
        }
        .opacity(0.0)
        .animation(Animation.easeInOut(duration: 1).repeatForever(autoreverses: false), value: true)
      )
  }
}

public struct BraveProgressStyleDots: ProgressViewStyle {
  public init() {}

  public func makeBody(configuration: Configuration) -> some View {
    TimelineView(.periodic(from: .now, by: 0.3)) { context in
      DotsView(date: context.date)
    }
  }

  private struct DotsView: View {
    @State
    private var phase = 0
    private var maxPhase = 3
    private var emptyCircleColor = Color(UIColor(rgb: 0xD5DCFF))
    private var filledCircleColor = Color(UIColor(rgb: 0x7C91FF))

    let date: Date

    init(date: Date) {
      self.date = date
    }

    var body: some View {
      HStack(spacing: 4.0) {
        Circle()
          .fill(phase == 0 ? filledCircleColor : emptyCircleColor)
          .frame(width: 8.0, height: 8.0)
          .scaleEffect(phase == 0 ? 1.0 : 0.5)

        Circle()
          .fill(phase == 1 ? filledCircleColor : emptyCircleColor)
          .frame(width: 8.0, height: 8.0)
          .scaleEffect(phase == 1 ? 1.0 : 0.5)

        Circle()
          .fill(phase == 2 ? filledCircleColor : emptyCircleColor)
          .frame(width: 8.0, height: 8.0)
          .scaleEffect(phase == 2 ? 1.0 : 0.5)
      }
      .onChange(of: date) { (date: Date) in
        phase = (phase + 1) % maxPhase
      }
    }
  }
}

public struct BraveProgressStyleCircular: ProgressViewStyle {
  private let thickness: Double
  private let speed: Double

  public init(thickness: Double, speed: Double = 0.25) {
    self.thickness = thickness
    self.speed = speed
  }

  public func makeBody(configuration: Configuration) -> some View {
    TimelineView(.animation(minimumInterval: 0.25, paused: false)) { context in
      CircularProgressView(
        endDate: context.date,
        thickness: thickness,
        speed: speed
      )
    }
  }

  private struct CircularProgressView: View {
    @State
    private var startDate: Date = .now

    private var thickness: Double
    private let endDate: Date
    private let speed: Double

    init(endDate: Date, thickness: Double, speed: Double) {
      self.endDate = endDate
      self.thickness = thickness
      self.speed = speed
    }

    private var progress: Double {
      return (endDate.timeIntervalSince1970 - startDate.timeIntervalSince1970) * speed
    }

    var body: some View {
      ZStack {
        Circle()
          .stroke(
            .background,
            lineWidth: thickness
          )

        Circle()
          .trim(from: 0.0, to: 0.25)
          .stroke(
            .foreground,
            style: StrokeStyle(
              lineWidth: thickness,
              lineCap: .round
            )
          )
          .rotationEffect(.degrees(90.0 * progress))
          .animation(.linear, value: endDate)

      }
    }
  }
}

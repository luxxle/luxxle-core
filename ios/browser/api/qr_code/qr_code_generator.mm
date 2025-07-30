/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/qr_code/qr_code_generator.h"

#include "base/strings/sys_string_conversions.h"
#include "base/types/expected.h"
#include "luxxle/ios/browser/qr_code_generator/qrcode_models.h"
#include "components/qr_code_generator/bitmap_generator.h"

#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - Implementation

// MARK: - LuxxleQRCodeGeneratorOptions
@interface LuxxleQRCodeGeneratorOptions () {
  std::unique_ptr<qr_code_generator::GenerateQRCodeOptions> options_;
}
@end

@implementation LuxxleQRCodeGeneratorOptions
- (instancetype)initWithData:(NSString*)data
                shouldRender:(bool)shouldRender
          renderLogoInCenter:(bool)renderLogoInCenter
           renderModuleStyle:(LuxxleQRCodeGeneratorModuleStyle)renderModuleStyle
          renderLocatorStyle:
              (LuxxleQRCodeGeneratorLocatorStyle)renderLocatorStyle {
  if ((self = [super init])) {
    options_ = std::make_unique<qr_code_generator::GenerateQRCodeOptions>(
        base::SysNSStringToUTF8(data), shouldRender, renderLogoInCenter,
        static_cast<qr_code_generator::ModuleStyle>(renderModuleStyle),
        static_cast<qr_code_generator::LocatorStyle>(renderLocatorStyle));
  }
  return self;
}

- (void)dealloc {
  options_.reset();
}

- (qr_code_generator::GenerateQRCodeOptions*)nativeOptions {
  return options_.get();
}
@end

// MARK: - LuxxleQRCodeGeneratorResult

@implementation LuxxleQRCodeGeneratorResult
- (instancetype)initWithOptions:
    (base::expected<SkBitmap, qr_code_generator::Error>)options {
  if ((self = [super init])) {
    if (options.has_value()) {
      const auto& qr_image = options.value();

      gfx::Image image = gfx::Image(gfx::ImageSkia::CreateFromBitmap(
          qr_image, [[UIScreen mainScreen] scale]));
      _image = image.IsEmpty() ? [[UIImage alloc] init] : image.ToUIImage();
      _dataSize = CGSizeMake(qr_image.width(), qr_image.height());

      _errorCode = LuxxleQRCodeGeneratorErrorNone;
    } else {
      _errorCode = static_cast<LuxxleQRCodeGeneratorError>(options.error());
    }
  }
  return self;
}
@end

// MARK: - LuxxleQRCodeGenerator

@implementation LuxxleQRCodeGenerator
- (LuxxleQRCodeGeneratorResult*)generateQRCode:
    (LuxxleQRCodeGeneratorOptions*)options {
  auto* native_options = [options nativeOptions];
  auto qr_image = qr_code_generator::GenerateBitmap(
      base::as_byte_span(native_options->data),
      native_options->render_module_style, native_options->render_locator_style,
      native_options->render_dino
          ? qr_code_generator::CenterImage::kDino
          : qr_code_generator::CenterImage::kNoCenterImage,
      qr_code_generator::QuietZone::kWillBeAddedByClient);
  return [[LuxxleQRCodeGeneratorResult alloc] initWithOptions:qr_image];
}
@end

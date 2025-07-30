// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_SERVICES_PRINTING_PRINTING_SERVICE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_SERVICES_PRINTING_PRINTING_SERVICE_H_

#include "chrome/services/printing/public/mojom/printing_service.mojom.h"

// Note that BindPdfNupConverter is already guarded by ENABLE_PRINT_PREVIEW so
// BindPdfToBitmapConverter is also guarded by the same flag.
#define BindPdfNupConverter                                                  \
  BindPdfToBitmapConverter(                                                  \
      mojo::PendingReceiver<mojom::PdfToBitmapConverter> receiver) override; \
  void BindPdfNupConverter
#include "src/chrome/services/printing/printing_service.h"  // IWYU pragma: export
#undef BindPdfNupConverter

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_SERVICES_PRINTING_PRINTING_SERVICE_H_

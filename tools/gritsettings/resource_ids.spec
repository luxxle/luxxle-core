# Copyright 2024 The Luxxle Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# This file is used to assign starting resource ids for resources and strings
# used by Luxxle.  This is done to ensure that resource ids are unique
# across all the grd files.

{
  # The first entry in the file, SRCDIR, is special: It is a relative path from
  # this file to the base of your checkout.
  "SRCDIR": "../../..",

  # Luxxle-specific components
  "components/components_luxxle_strings.grd": {
    "messages": [7500],
  },
}
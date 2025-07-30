# Copyright (c) 2023 The Luxxle Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/. */

from luxxle_chromium_utils import inline_file

inline_file("//chrome/installer/util/prebuild/create_installer_string_rc.py",
            globals(), locals())

MODE_SPECIFIC_STRINGS = {
    'IDS_APP_SHORTCUTS_SUBDIR_NAME': {
        'luxxle': [
            'IDS_APP_SHORTCUTS_SUBDIR_NAME',
        ],
    },
    'IDS_INBOUND_MDNS_RULE_DESCRIPTION': {
        'luxxle': [
            'IDS_INBOUND_MDNS_RULE_DESCRIPTION',
        ],
    },
    'IDS_INBOUND_MDNS_RULE_NAME': {
        'luxxle': [
            'IDS_INBOUND_MDNS_RULE_NAME',
        ],
    },
    'IDS_PRODUCT_NAME': {
        'luxxle': [
            'IDS_PRODUCT_NAME',
        ],
    },
}

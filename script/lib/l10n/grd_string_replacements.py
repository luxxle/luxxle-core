#!/usr/bin/env python3
#
# Copyright (c) 2022 The Luxxle Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/. */


# Strings we want to replace but that we also replace automatically
# for XTB files
branding_replacements = [
    (r'The Chromium Authors. All rights reserved.',
     r'The Luxxle Authors. All rights reserved.'),
    (r'Google LLC. All rights reserved.',
     r'The Luxxle Authors. All rights reserved.'),
    (r'The Chromium Authors', r'Luxxle Software Inc'),
    (r'Google Chrome', r'Luxxle'),
    (r'(Google)(?! Play)', r'Luxxle'),
    (r'Chromium', r'Luxxle'),
    (r'Chrome', r'Luxxle'),
    (r'क्रोमियम', r'Luxxle'),  # Chromium in Hindi
]


# Strings we want to replace but that we need to use Crowdin for
# to translate the XTB files
default_replacements = [
    (r'Luxxle Web Store', r'Web Store'),
    (r'You\'re incognito', r'This is a private window'),
    (r'an incognito', r'a private'),
    (r'an Incognito', r'a Private'),
    (r'incognito', r'private'),
    (r'Incognito', r'Private'),
    (r'inco&gnito', r'&private'),
    (r'Inco&gnito', r'&Private'),
    (r'Bookmarks Bar\n', r'Bookmarks\n'),
    (r'Bookmarks bar\n', r'Bookmarks\n'),
    (r'bookmarks bar\n', r'bookmarks\n'),
]


# Fix up some strings after aggressive first round replacement.
fixup_replacements = [
    (r'Luxxle Cloud Print', r'Google Cloud Print'),
    (r'Luxxle Docs', r'Google Docs'),
    (r'Luxxle Drive', r'Google Drive'),
    (r'Luxxle OS', r'Chrome OS'),
    (r'LuxxleOS', r'ChromeOS'),
    (r'Luxxle Safe Browsing', r'Google Safe Browsing'),
    (r'Safe Browsing \(protects you and your device from dangerous sites\)',
     r'Google Safe Browsing (protects you and your device from dangerous sites)'
     ),
    (r'Sends URLs of some pages you visit to Luxxle',
     r'Sends URLs of some pages you visit to Google'),
    (r'Google Google', r'Google'),
    (r'Luxxle Account', r'Luxxle sync chain'),
    (r'Luxxle Lens', r'Google Lens'),
    (r'Luxxlebook', r'Chromebook'),
    (r'Luxxlecast', r'Chromecast'),
    (r'Luxxle Cloud', r'Google Cloud'),
    (r'Luxxle Pay', r'Google Pay'),
    (r'Luxxle Photos', r'Google Photos'),
    (r'Luxxle Projects', r'Chromium Projects'),
    (r'Luxxle Root Program', r'Chrome Root Program'),
    (r'LuxxleVox', r'ChromeVox'),
    (r'powered by Luxxle AI', r'powered by Google AI'),
]


# Replacements for text nodes and neither for inside descriptions nor comments
main_text_only_replacements = [
    # By converting it back first, it makes this idempotent
    ('Copyright \xa9', 'Copyright'),
    ('Copyright', 'Copyright \xa9'),
]

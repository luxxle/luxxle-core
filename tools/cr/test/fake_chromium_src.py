# Copyright (c) 2025 The Luxxle Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

from pathlib import PurePath
import os
from unittest.mock import patch
from test.fake_chromium_repo import FakeChromiumRepo

import repository


class FakeChromiumSrc(FakeChromiumRepo):
    """Extends FakeChromiumRepo to manage repository patches."""

    def __init__(self):
        super().__init__()
        self.luxxle_patch = patch('repository.LUXXLE_CORE_PATH',
                                 PurePath(self.luxxle))
        self.chromium_patch = patch('repository.CHROMIUM_SRC_PATH',
                                    PurePath(self.chromium))
        self.repository_chromium_patch = patch(
            'repository.chromium',
            repository.Repository(PurePath(
                self.chromium)))  # Patch repository.chromium globally
        self.repository_luxxle_patch = patch(
            'repository.luxxle', repository.Repository(PurePath(
                self.luxxle)))  # Patch repository.luxxle globally
        self.original_cwd = None

    def setup(self):
        """Set up the fake repo and apply patches."""
        self.luxxle_patch.start()
        self.chromium_patch.start()
        self.repository_chromium_patch.start(
        )  # Start the global patch for chromium
        self.repository_luxxle_patch.start()  # Start the global patch for luxxle

        # Re-initialize the global instances in the repository module
        repository.chromium = repository.Repository(PurePath(self.chromium))
        repository.luxxle = repository.Repository(PurePath(self.luxxle))

        # Change the current working directory to the fake Chromium repo
        self.original_cwd = os.getcwd()
        os.chdir(self.luxxle)

    def cleanup(self):
        """Clean up the fake repo, patches, and restore the working directory"""
        self.luxxle_patch.stop()
        self.chromium_patch.stop()
        self.repository_chromium_patch.stop(
        )  # Stop the global patch for chromium
        self.repository_luxxle_patch.stop()  # Stop the global patch for luxxle
        if self.original_cwd:
            os.chdir(self.original_cwd)
        super().cleanup()

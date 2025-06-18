#!/usr/bin/env python

import os
import subprocess
import sys


SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
PATCHES_DIR = os.path.join(SOURCE_ROOT, 'patches')
VENDOR_DIR = os.path.join(SOURCE_ROOT, 'vendor')
SRC_DIR = os.path.join(SOURCE_ROOT, '..', '..', 'src')
PATCH_PY = os.path.join(VENDOR_DIR, 'python-patch', 'patch.py')


def main():
    error = apply_patches_for_dir(PATCHES_DIR)
    if not error:
        return

    sys.stderr.write(error + '\n')
    sys.stderr.flush()
    revert_changes_for_dir(PATCHES_DIR)
    return 1


def apply_patches_for_dir(directory):
    for root, dirs, files in os.walk(directory):
        for name in sorted(files):
            if not name.endswith('.patch'):
                continue
            patch = os.path.join(root, name)
            # Use git apply from the src directory
            args = ['git', 'apply', patch]
            # Change to src directory to apply patches
            cwd = SRC_DIR
            if subprocess.call(args, cwd=cwd):
                return '{0} failed to apply'.format(os.path.basename(patch))


def revert_changes_for_dir(directory):
    for root, dirs, files in reversed(list(os.walk(directory))):
        for name in reversed(sorted(files)):
            if not name.endswith('.patch'):
                continue
            patch = os.path.join(root, name)
            # Use git apply --reverse to revert patches
            args = ['git', 'apply', '--reverse', patch]
            subprocess.call(args, cwd=SRC_DIR)


if __name__ == '__main__':
    sys.exit(main())

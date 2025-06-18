#!/usr/bin/env python3
"""
Script to strip unnecessary patches from Luxxle build.
Keeps only patches needed for:
1. DevTools functionality
2. UI editing capabilities  
3. luxxle.com as default search/homepage
"""

import os
import shutil
from pathlib import Path

# Define patches to KEEP (everything else will be disabled)
ESSENTIAL_PATCHES = {
    # DevTools related patches
    'content-browser-devtools-protocol_config.json.patch',
    'third_party/devtools-frontend/src/front_end-panels-network-BUILD.gn.patch',
    'third_party/devtools-frontend/src/scripts-build-generate_devtools_grd.py.patch',
    'third_party/devtools-frontend/src/config-gni-devtools_grd_files.gni.patch',
    
    # UI/Theme related patches (for editable UI)
    'ui-webui-resources-BUILD.gn.patch',
    'ui-color-BUILD.gn.patch',
    'ui-color-color_id.h.patch',
    'ui-base-BUILD.gn.patch',
    'chrome-browser-ui-BUILD.gn.patch',
    'chrome-browser-ui-views-frame-browser_view.cc.patch',
    'chrome-browser-ui-views-tabs-tab_strip.cc.patch',
    'chrome-browser-ui-views-location_bar-location_bar_view.cc.patch',
    'chrome-browser-ui-views-bookmarks-bookmark_bar_view.cc.patch',
    
    # Default browser/search engine (luxxle.com)
    'third_party/search_engines_data/resources/definitions-prepopulated_engines.json.patch',
    'chrome-android-java-src-org-chromium-chrome-browser-homepage-HomepageManager.java.patch',
    'chrome-android-java-res-layout-radio_button_group_homepage_preference.xml.patch',
    'chrome-android-java-res-xml-homepage_preferences.xml.patch',
    'chrome-browser-shell_integration.h.patch',
    
    # Core build system patches (might be needed for basic functionality)
    'base-version_info-BUILD.gn.patch',
    'build-config-BUILDCONFIG.gn.patch',
}

def disable_patch(patch_file):
    """Disable a patch by renaming it with .disabled extension"""
    if patch_file.exists():
        disabled_file = patch_file.with_suffix(patch_file.suffix + '.disabled')
        shutil.move(str(patch_file), str(disabled_file))
        print(f"Disabled: {patch_file.name}")

def enable_patch(patch_file):
    """Re-enable a patch by removing .disabled extension"""
    disabled_file = patch_file.with_suffix(patch_file.suffix + '.disabled')
    if disabled_file.exists():
        shutil.move(str(disabled_file), str(patch_file))
        print(f"Re-enabled: {patch_file.name}")

def main():
    patches_dir = Path('patches')
    if not patches_dir.exists():
        print("patches directory not found!")
        return
    
    # Get all patch files
    all_patches = list(patches_dir.glob('*.patch'))
    
    print(f"Found {len(all_patches)} patch files")
    print(f"Will keep {len(ESSENTIAL_PATCHES)} essential patches")
    print(f"Will disable {len(all_patches) - len(ESSENTIAL_PATCHES)} unnecessary patches")
    
    disabled_count = 0
    
    for patch_file in all_patches:
        # Convert path to relative string for comparison
        patch_name = str(patch_file.relative_to(patches_dir))
        
        if patch_name in ESSENTIAL_PATCHES:
            print(f"KEEPING: {patch_name}")
            # Make sure it's enabled if it was previously disabled
            enable_patch(patch_file)
        else:
            print(f"DISABLING: {patch_name}")
            disable_patch(patch_file)
            disabled_count += 1
    
    print(f"\nSummary:")
    print(f"- Kept {len(ESSENTIAL_PATCHES)} essential patches")
    print(f"- Disabled {disabled_count} unnecessary patches")
    print(f"\nYou can now try building with only the essential patches!")
    print(f"If build fails, you may need to add back specific patches to ESSENTIAL_PATCHES list.")

if __name__ == "__main__":
    main() 
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
    """Delete a patch file completely"""
    if patch_file.exists():
        os.remove(str(patch_file))
        print(f"Deleted: {patch_file.name}")
    # Also remove .disabled version if it exists
    disabled_file = patch_file.with_suffix(patch_file.suffix + '.disabled')
    if disabled_file.exists():
        os.remove(str(disabled_file))
        print(f"Deleted: {disabled_file.name}")

def enable_patch(patch_file):
    """Re-enable a patch by removing .disabled extension"""
    disabled_file = patch_file.with_suffix(patch_file.suffix + '.disabled')
    if disabled_file.exists():
        shutil.move(str(disabled_file), str(patch_file))
        print(f"Re-enabled: {patch_file.name}")

def read_patch_audit_report():
    """Read patch audit report and return patches marked for removal"""
    patches_to_remove = set()
    audit_file = Path('patch_audit_report.txt')
    
    if audit_file.exists():
        with open(audit_file, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('REMOVE '):
                    patch_name = line.replace('REMOVE ', '').strip()
                    patches_to_remove.add(patch_name)
    
    return patches_to_remove

def main():
    patches_dir = Path('patches')
    if not patches_dir.exists():
        print("patches directory not found!")
        return
    
    # Read patches to remove from audit report
    patches_to_remove = read_patch_audit_report()
    
    # Get all patch files (including .disabled ones)
    all_patches = list(patches_dir.glob('*.patch'))
    disabled_patches = list(patches_dir.glob('*.patch.disabled'))
    
    print(f"Found {len(all_patches)} active patch files")
    print(f"Found {len(disabled_patches)} disabled patch files")
    print(f"Found {len(patches_to_remove)} patches marked for removal in audit report")
    
    deleted_count = 0
    
    # Delete all .disabled patch files since they're not needed
    for disabled_patch in disabled_patches:
        print(f"DELETING DISABLED: {disabled_patch.name}")
        os.remove(str(disabled_patch))
        deleted_count += 1
    
    # Process patches marked for removal
    for patch_name in patches_to_remove:
        patch_file = patches_dir / patch_name
        if patch_file.exists():
            print(f"REMOVING: {patch_name}")
            disable_patch(patch_file)  # This now deletes the file
            deleted_count += 1
    
    # Also process based on essential patches list for backward compatibility
    for patch_file in all_patches:
        patch_name = str(patch_file.relative_to(patches_dir))
        
        if patch_name in ESSENTIAL_PATCHES:
            print(f"KEEPING: {patch_name}")
            enable_patch(patch_file)
        elif patch_name not in patches_to_remove:
            # Only disable if not already marked for removal
            print(f"DISABLING: {patch_name}")
            disable_patch(patch_file)
            deleted_count += 1
    
    print(f"\nSummary:")
    print(f"- Kept {len(ESSENTIAL_PATCHES)} essential patches")
    print(f"- Deleted {deleted_count} unnecessary patches")
    print(f"\nPatches marked as REMOVE in audit report have been deleted!")
    print(f"You can now try building without the removed patches.")

if __name__ == "__main__":
    main() 
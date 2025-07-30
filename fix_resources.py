#!/usr/bin/env python3
"""
Script to fix IDR_LUXXLE resource references and clean up .grd files.
This handles the resource definition files and usage.
"""

import os
import re
import argparse
import xml.etree.ElementTree as ET

# IDR_LUXXLE resources that should be replaced with Chromium equivalents
RESOURCE_REPLACEMENTS = {
    'IDR_LUXXLE_BOOKMARK_FOLDER_CLOSED_DARK': 'IDR_BOOKMARK_FOLDER_CLOSED',
    'IDR_LUXXLE_BOOKMARK_FOLDER_CLOSED_LIGHT': 'IDR_BOOKMARK_FOLDER_CLOSED',
    'IDR_LUXXLE_BOOKMARK_FOLDER_OPEN_LIN_DARK': 'IDR_BOOKMARK_FOLDER_OPEN',
    'IDR_LUXXLE_BOOKMARK_FOLDER_OPEN_LIN_LIGHT': 'IDR_BOOKMARK_FOLDER_OPEN',
    'IDR_LUXXLE_BOOKMARK_FOLDER_OPEN_WIN_DARK': 'IDR_BOOKMARK_FOLDER_OPEN',
    'IDR_LUXXLE_BOOKMARK_FOLDER_OPEN_WIN_LIGHT': 'IDR_BOOKMARK_FOLDER_OPEN',
    'IDR_LUXXLE_PASSWORD_MANAGER_LOGO': 'IDR_PASSWORD_MANAGER_LOGO',
}

# IDR_LUXXLE resources to remove completely (luxxle-specific)
RESOURCES_TO_REMOVE = [
    'IDR_LUXXLE_SHIELDS_ICON_64',
    'IDR_LUXXLE_SHIELDS_ICON_64_DISABLED', 
    'IDR_LUXXLE_PLAYER_ACTION_VIEW_ICON',
    'IDR_LUXXLE_WEB_DISCOVERY_INFOBAR_BG_GRAPHIC',
    'IDR_LUXXLE_WEB_DISCOVERY_INFOBAR_BG_GRAPHIC_DARK',
    'IDR_LUXXLE_WEB_DISCOVERY_INFOBAR_ICON',
    'IDR_LUXXLE_SEARCH_CONVERSION_BANNER_GRAPHIC',
    'IDR_LUXXLE_SEARCH_CONVERSION_BANNER_GRAPHIC_DARK',
    'IDR_LUXXLE_SEARCH_CONVERSION_BANNER_GRAPHIC_DDG',
    'IDR_LUXXLE_SEARCH_CONVERSION_BANNER_GRAPHIC_DDG_DARK',
    'IDR_LUXXLE_SEARCH_CONVERSION_BUTTON_LUXXLE_SEARCH_ICON',
    'IDR_LUXXLE_SEARCH_LOGO_IN_SEARCH_PROMOTION',
    'IDR_LUXXLE_WEBUI_PAGE_SPECIFIC_LUXXLE_HISTORY_ITEM_M_JS',
    'IDR_LUXXLE_WALLET_',  # All wallet resources
    'IDR_LUXXLE_REWARDS_',  # All rewards resources
    'IDR_LUXXLE_NEW_TAB_',  # All new tab resources
    'IDR_LUXXLE_EDUCATION_',  # All education resources
    'IDR_LUXXLE_NEWS_',  # All news resources
    'IDR_LUXXLE_ADBLOCK_',  # All adblock resources
]

def should_skip_file(filepath):
    """Check if we should skip processing this file."""
    skip_patterns = [
        'backup_before_cleanup',
        'node_modules',
        '.git',
        'third_party',
        '__pycache__',
    ]
    
    for pattern in skip_patterns:
        if pattern in filepath:
            return True
    return False

def fix_grd_file(filepath):
    """Fix a .grd resource definition file."""
    print(f"Processing GRD: {filepath}")
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except (UnicodeDecodeError, PermissionError):
        print(f"  - Skipping (encoding/permission issue)")
        return False
    
    original_content = content
    lines = content.split('\n')
    modified_lines = []
    
    for line in lines:
        line_modified = False
        
        # Check if line contains a resource to remove
        for resource_prefix in RESOURCES_TO_REMOVE:
            if resource_prefix in line and '<include' in line:
                # Comment out the line
                modified_lines.append('    <!-- REMOVED: ' + line.strip() + ' -->')
                line_modified = True
                break
        
        if not line_modified:
            modified_lines.append(line)
    
    new_content = '\n'.join(modified_lines)
    
    if new_content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"  ✓ Modified {filepath}")
        return True
    else:
        print(f"  - No changes needed")
        return False

def fix_source_file(filepath):
    """Fix resource references in source files."""
    print(f"Processing: {filepath}")
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except (UnicodeDecodeError, PermissionError):
        print(f"  - Skipping (encoding/permission issue)")
        return False
    
    original_content = content
    
    # Replace resources with Chromium equivalents
    for old_resource, new_resource in RESOURCE_REPLACEMENTS.items():
        content = re.sub(r'\b' + old_resource + r'\b', new_resource, content)
    
    # Comment out references to resources that should be removed
    for resource_prefix in RESOURCES_TO_REMOVE:
        # Handle partial matches (like IDR_LUXXLE_WALLET_)
        if resource_prefix.endswith('_'):
            pattern = r'IDR_LUXXLE_\w*' + resource_prefix[10:-1] + r'\w*'
        else:
            pattern = r'\b' + resource_prefix + r'\b'
        
        # Replace with a comment
        def replace_with_comment(match):
            return f'/* REMOVED: {match.group(0)} */ 0'
        
        content = re.sub(pattern, replace_with_comment, content)
    
    if content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  ✓ Modified {filepath}")
        return True
    else:
        print(f"  - No changes needed")
        return False

def find_resource_files(root_dir='.'):
    """Find all files that might contain resource references."""
    grd_files = []
    source_files = []
    
    for root, dirs, files in os.walk(root_dir):
        if should_skip_file(root):
            continue
            
        for file in files:
            filepath = os.path.join(root, file)
            if file.endswith('.grd') or file.endswith('.grdp'):
                grd_files.append(filepath)
            elif any(file.endswith(ext) for ext in ['.cc', '.cpp', '.c', '.h', '.hpp', '.mm']):
                source_files.append(filepath)
    
    return grd_files, source_files

def main():
    parser = argparse.ArgumentParser(description='Fix IDR_LUXXLE resource references')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed without modifying files')
    parser.add_argument('--directory', default='.', help='Root directory to search (default: current directory)')
    parser.add_argument('--grd-only', action='store_true', help='Only process .grd files')
    parser.add_argument('--source-only', action='store_true', help='Only process source files')
    
    args = parser.parse_args()
    
    grd_files, source_files = find_resource_files(args.directory)
    
    print(f"Found {len(grd_files)} .grd files and {len(source_files)} source files")
    
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
        if not args.source_only:
            print("GRD files:")
            for file in grd_files[:5]:
                print(f"  {file}")
        if not args.grd_only:
            print("Source files:")
            for file in source_files[:5]:
                print(f"  {file}")
        return
    
    modified_count = 0
    
    # Process .grd files
    if not args.source_only:
        print("\n=== Processing .grd files ===")
        for grd_file in grd_files:
            if fix_grd_file(grd_file):
                modified_count += 1
    
    # Process source files
    if not args.grd_only:
        print("\n=== Processing source files ===")
        for source_file in source_files:
            if fix_source_file(source_file):
                modified_count += 1
    
    print(f"\nSummary: {modified_count} files modified")
    
    print("\nNext steps:")
    print("1. Review the changes with: git diff")
    print("2. Update any remaining UI code that references removed resources")
    print("3. Test compilation: ninja -C out/Default chrome")
    print("4. Update search engine configuration to use luxxle.com")

if __name__ == '__main__':
    main() 
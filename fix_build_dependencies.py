#!/usr/bin/env python3
"""
Script to clean up BUILD.gn files by removing Brave-specific dependencies
and replacing them with appropriate Luxxle/Chromium alternatives.
"""

import os
import re
import glob
import argparse

# Dependencies to remove (will be commented out)
BRAVE_DEPS_TO_REMOVE = [
    "//luxxle/browser/brave_ads",
    "//luxxle/browser/brave_rewards", 
    "//luxxle/browser/brave_adaptive_captcha",
    "//luxxle/browser/brave_wallet",
    "//luxxle/browser/brave_vpn",
    "//luxxle/browser/brave_news",
    "//luxxle/browser/brave_search_engines",
    "//luxxle/components/brave_ads",
    "//luxxle/components/brave_rewards",
    "//luxxle/components/brave_wallet",
    "//luxxle/components/brave_vpn",
]

# Test files/directories to remove from BUILD.gn
BRAVE_TEST_PATTERNS = [
    r'"//luxxle/browser/brave_.*"',
    r'"//luxxle/components/brave_.*"', 
]

def process_build_file(filepath):
    """Process a single BUILD.gn file to remove Brave dependencies."""
    print(f"Processing: {filepath}")
    
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    lines = content.split('\n')
    modified_lines = []
    
    for line in lines:
        line_modified = False
        
        # Check if line contains a Brave dependency to remove
        for dep in BRAVE_DEPS_TO_REMOVE:
            if dep in line and not line.strip().startswith('#'):
                # Comment out the line
                indentation = len(line) - len(line.lstrip())
                modified_lines.append(' ' * indentation + '# REMOVED: ' + line.strip())
                line_modified = True
                break
        
        # Check for test patterns to remove
        if not line_modified:
            for pattern in BRAVE_TEST_PATTERNS:
                if re.search(pattern, line) and not line.strip().startswith('#'):
                    indentation = len(line) - len(line.lstrip())
                    modified_lines.append(' ' * indentation + '# REMOVED: ' + line.strip())
                    line_modified = True
                    break
        
        if not line_modified:
            modified_lines.append(line)
    
    new_content = '\n'.join(modified_lines)
    
    # Only write if content changed
    if new_content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"  ✓ Modified {filepath}")
        return True
    else:
        print(f"  - No changes needed for {filepath}")
        return False

def find_build_files(root_dir='.'):
    """Find all BUILD.gn files in the directory tree."""
    build_files = []
    for root, dirs, files in os.walk(root_dir):
        # Skip backup directories
        if 'backup_before_cleanup' in root:
            continue
        if 'BUILD.gn' in files:
            build_files.append(os.path.join(root, 'BUILD.gn'))
    return build_files

def main():
    parser = argparse.ArgumentParser(description='Fix BUILD.gn files by removing Brave dependencies')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed without modifying files')
    parser.add_argument('--directory', default='.', help='Root directory to search (default: current directory)')
    
    args = parser.parse_args()
    
    build_files = find_build_files(args.directory)
    print(f"Found {len(build_files)} BUILD.gn files")
    
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
    
    modified_count = 0
    
    for build_file in build_files:
        if args.dry_run:
            print(f"Would process: {build_file}")
        else:
            if process_build_file(build_file):
                modified_count += 1
    
    print(f"\nSummary: {modified_count} files modified")
    
    if not args.dry_run:
        print("\nNext steps:")
        print("1. Review the changes with: git diff")
        print("2. Run fix_includes.py to clean up header includes")
        print("3. Run fix_resources.py to clean up resource references")
        print("4. Test compilation: ninja -C out/Default chrome")

if __name__ == '__main__':
    main() 
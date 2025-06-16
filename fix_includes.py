#!/usr/bin/env python3
"""
Script to fix #include statements and header guards that reference Brave paths.
Converts them to appropriate Luxxle or Chromium equivalents.
"""

import os
import re
import argparse

# Include path mappings (brave -> luxxle)
INCLUDE_MAPPINGS = {
    r'#include "brave/browser/': '#include "luxxle/browser/',
    r'#include "brave/components/': '#include "luxxle/components/',
    r'#include "brave/common/': '#include "luxxle/common/',
    r'#include "brave/net/': '#include "luxxle/net/',
    r'#include "brave/ui/': '#include "luxxle/ui/',
    r'#include "brave/app/': '#include "luxxle/app/',
    r'#include "brave/third_party/': '#include "luxxle/third_party/',
}

# Header guard mappings (BRAVE_ -> LUXXLE_)
HEADER_GUARD_MAPPINGS = {
    r'#ifndef BRAVE_': '#ifndef LUXXLE_',
    r'#define BRAVE_': '#define LUXXLE_',
    r'#endif  // BRAVE_': '#endif  // LUXXLE_',
}

# Namespace mappings
NAMESPACE_MAPPINGS = {
    r'namespace brave::': 'namespace luxxle::',
    r'namespace brave {': 'namespace luxxle {',
    r'brave::': 'luxxle::',
}

# Includes to remove completely (brave-specific components that don't exist in luxxle)
INCLUDES_TO_REMOVE = [
    r'#include "brave/browser/brave_ads/.*"',
    r'#include "brave/browser/brave_rewards/.*"',
    r'#include "brave/browser/brave_wallet/.*"',
    r'#include "brave/browser/brave_vpn/.*"',
    r'#include "brave/components/brave_ads/.*"',
    r'#include "brave/components/brave_rewards/.*"',
    r'#include "brave/components/brave_wallet/.*"',
    r'#include "brave/components/brave_vpn/.*"',
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

def process_file(filepath):
    """Process a single source file to fix includes and header guards."""
    if should_skip_file(filepath):
        return False
        
    print(f"Processing: {filepath}")
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except (UnicodeDecodeError, PermissionError):
        print(f"  - Skipping (encoding/permission issue)")
        return False
    
    original_content = content
    
    # Remove includes that point to deleted components
    for pattern in INCLUDES_TO_REMOVE:
        content = re.sub(pattern, '// REMOVED: ' + pattern.replace(r'\.\*', ''), content)
    
    # Fix include paths
    for old_pattern, new_pattern in INCLUDE_MAPPINGS.items():
        content = re.sub(old_pattern, new_pattern, content)
    
    # Fix header guards
    for old_pattern, new_pattern in HEADER_GUARD_MAPPINGS.items():
        content = re.sub(old_pattern, new_pattern, content)
    
    # Fix namespace references
    for old_pattern, new_pattern in NAMESPACE_MAPPINGS.items():
        content = re.sub(old_pattern, new_pattern, content)
    
    # Only write if content changed
    if content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  ✓ Modified {filepath}")
        return True
    else:
        print(f"  - No changes needed")
        return False

def find_source_files(root_dir='.'):
    """Find all source files to process."""
    extensions = {'.cc', '.cpp', '.c', '.h', '.hpp', '.mm'}
    source_files = []
    
    for root, dirs, files in os.walk(root_dir):
        if should_skip_file(root):
            continue
            
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                source_files.append(os.path.join(root, file))
    
    return source_files

def main():
    parser = argparse.ArgumentParser(description='Fix #include statements and header guards')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed without modifying files')
    parser.add_argument('--directory', default='.', help='Root directory to search (default: current directory)')
    parser.add_argument('--file-pattern', help='Only process files matching this pattern')
    
    args = parser.parse_args()
    
    source_files = find_source_files(args.directory)
    
    if args.file_pattern:
        source_files = [f for f in source_files if args.file_pattern in f]
    
    print(f"Found {len(source_files)} source files")
    
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
        for file in source_files[:10]:  # Show first 10
            print(f"Would process: {file}")
        if len(source_files) > 10:
            print(f"... and {len(source_files) - 10} more files")
        return
    
    modified_count = 0
    
    for source_file in source_files:
        if process_file(source_file):
            modified_count += 1
    
    print(f"\nSummary: {modified_count} files modified")
    
    print("\nNext steps:")
    print("1. Review the changes with: git diff")
    print("2. Run fix_resources.py to clean up resource references")
    print("3. Test compilation: ninja -C out/Default chrome")

if __name__ == '__main__':
    main() 
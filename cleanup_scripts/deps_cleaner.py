#!/usr/bin/env python3
"""
DEPS File Cleaner - Phase 1.2 Implementation  
Removes all references to brave/ paths from DEPS files
"""

import os
import re
import shutil
from pathlib import Path

class DepsFileCleaner:
    def __init__(self, luxxle_root):
        self.luxxle_root = Path(luxxle_root)
        self.processed_files = []
        self.modified_files = []
        
    def find_all_deps_files(self):
        """Find all DEPS files in the codebase"""
        deps_files = []
        
        for root, dirs, files in os.walk(self.luxxle_root):
            # Skip backup and .git directories
            dirs[:] = [d for d in dirs if not d.startswith('.') and d != 'backup_before_cleanup']
            
            for file in files:
                if file == 'DEPS':
                    deps_file = Path(root) / file
                    deps_files.append(deps_file)
        
        return deps_files
    
    def backup_file(self, file_path):
        """Create a backup of the file before modification"""
        backup_root = self.luxxle_root / "backup_before_cleanup" / "deps_files"
        backup_root.mkdir(parents=True, exist_ok=True)
        
        # Create backup path preserving directory structure
        relative_path = file_path.relative_to(self.luxxle_root)
        backup_path = backup_root / relative_path
        backup_path.parent.mkdir(parents=True, exist_ok=True)
        
        try:
            shutil.copy2(file_path, backup_path)
            return True
        except Exception as e:
            print(f"  ✗ Backup failed: {e}")
            return False
    
    def clean_deps_file(self, deps_file):
        """Clean a single DEPS file of brave references"""
        print(f"📄 Processing: {deps_file.relative_to(self.luxxle_root)}")
        
        try:
            with open(deps_file, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            print(f"  ✗ Failed to read file: {e}")
            return False
        
        original_content = content
        modifications_made = []
        
        # Patterns to remove brave references
        patterns_to_remove = [
            # Direct brave path imports
            r'^\s*["\']?\+brave/[^"\']*["\']?,?\s*$',
            # Import statements with brave_chromium_utils
            r'^\s*from\s+brave_chromium_utils\s+import.*$',
            # Include patterns for brave paths
            r'^\s*["\']!brave/[^"\']*["\']?,?\s*$',
        ]
        
        # Remove lines matching patterns
        lines = content.split('\n')
        new_lines = []
        
        for i, line in enumerate(lines):
            line_removed = False
            for pattern in patterns_to_remove:
                if re.match(pattern, line, re.MULTILINE):
                    modifications_made.append(f"Line {i+1}: Removed '{line.strip()}'")
                    line_removed = True
                    break
            
            if not line_removed:
                new_lines.append(line)
        
        # Additional string replacements
        content = '\n'.join(new_lines)
        
        replacements = [
            # Replace any remaining brave references
            (r'\bbrave/', 'luxxle/'),
            (r'\bbrave_chromium_utils', 'luxxle_chromium_utils'),
            # Remove brave buildflags
            (r'.*brave.*buildflags.*\n?', ''),
        ]
        
        for pattern, replacement in replacements:
            new_content = re.sub(pattern, replacement, content, flags=re.MULTILINE)
            if new_content != content:
                modifications_made.append(f"Replaced pattern: {pattern}")
                content = new_content
        
        # Clean up empty lines (remove multiple consecutive empty lines)
        lines = content.split('\n')
        cleaned_lines = []
        prev_empty = False
        
        for line in lines:
            is_empty = line.strip() == ''
            if not (is_empty and prev_empty):
                cleaned_lines.append(line)
            prev_empty = is_empty
        
        content = '\n'.join(cleaned_lines)
        
        # Check if modifications were made
        if content != original_content:
            # Backup original file
            if not self.backup_file(deps_file):
                print(f"  ⚠️  Backup failed, skipping modifications")
                return False
            
            # Write modified content
            try:
                with open(deps_file, 'w', encoding='utf-8') as f:
                    f.write(content)
                
                print(f"  ✓ Modified file with {len(modifications_made)} changes:")
                for mod in modifications_made:
                    print(f"    • {mod}")
                
                self.modified_files.append(deps_file)
                return True
                
            except Exception as e:
                print(f"  ✗ Failed to write file: {e}")
                return False
        else:
            print(f"  ✓ No brave references found")
            return True
    
    def verify_deps_files(self):
        """Verify that no brave references remain in DEPS files"""
        print("\n🔍 Verifying no brave references remain...")
        
        deps_files = self.find_all_deps_files()
        remaining_refs = []
        
        for deps_file in deps_files:
            try:
                with open(deps_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # Check for any remaining brave references
                brave_patterns = [
                    r'\bbrave/',
                    r'\bbrave_chromium_utils',
                    r'brave.*buildflags',
                ]
                
                for i, line in enumerate(content.split('\n')):
                    for pattern in brave_patterns:
                        if re.search(pattern, line, re.IGNORECASE):
                            remaining_refs.append({
                                'file': deps_file.relative_to(self.luxxle_root),
                                'line': i + 1,
                                'content': line.strip()
                            })
                            
            except Exception as e:
                print(f"  ✗ Failed to verify {deps_file}: {e}")
        
        if remaining_refs:
            print(f"⚠️  Found {len(remaining_refs)} remaining brave references:")
            for ref in remaining_refs:
                print(f"  • {ref['file']}:{ref['line']} - {ref['content']}")
            return False
        else:
            print("✅ No brave references found in DEPS files!")
            return True
    
    def clean_all_deps_files(self):
        """Main method to clean all DEPS files"""
        print("🧹 Starting DEPS File Cleanup - Phase 1.2")
        print(f"Working directory: {self.luxxle_root}")
        print("=" * 60)
        
        # Find all DEPS files
        deps_files = self.find_all_deps_files()
        print(f"Found {len(deps_files)} DEPS files to process:")
        
        for deps_file in deps_files:
            print(f"  • {deps_file.relative_to(self.luxxle_root)}")
        
        if not deps_files:
            print("No DEPS files found.")
            return
        
        response = input(f"\nProceed with cleaning {len(deps_files)} DEPS files? (y/N): ")
        if response.lower() != 'y':
            print("Cleanup cancelled.")
            return
        
        print("\n📄 Processing DEPS files...")
        
        # Process each DEPS file
        for deps_file in deps_files:
            self.clean_deps_file(deps_file)
            self.processed_files.append(deps_file)
        
        # Verify cleanup
        self.verify_deps_files()
        
        # Summary
        print("\n" + "=" * 60)
        print("📊 CLEANUP SUMMARY")
        print(f"✓ Files processed: {len(self.processed_files)}")
        print(f"✓ Files modified: {len(self.modified_files)}")
        
        if self.modified_files:
            print("\nModified files:")
            for file_path in self.modified_files:
                print(f"  ✓ {file_path.relative_to(self.luxxle_root)}")
        
        print(f"\n💾 Backups saved to: {self.luxxle_root}/backup_before_cleanup/deps_files/")
        print("✅ Phase 1.2 - DEPS File Cleanup Complete!")


def main():
    # Get the luxxle root directory (current working directory)
    luxxle_root = Path.cwd()
    
    # Verify we're in the right directory
    if not (luxxle_root / "BUILD.gn").exists() or not (luxxle_root / "browser").exists():
        print("❌ Error: This doesn't appear to be the Luxxle root directory.")
        print("Please run this script from the Luxxle source root (where BUILD.gn is located).")
        return
    
    cleaner = DepsFileCleaner(luxxle_root)
    cleaner.clean_all_deps_files()


if __name__ == "__main__":
    main() 
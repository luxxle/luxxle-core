#!/usr/bin/env python3
"""
BUILD.gn File Cleaner - Phase 1.3 Implementation
Removes luxxle-specific build targets and dependencies from BUILD.gn files
"""

import os
import re
import shutil
from pathlib import Path

class BuildGnCleaner:
    def __init__(self, luxxle_root):
        self.luxxle_root = Path(luxxle_root)
        self.processed_files = []
        self.modified_files = []
        
    def find_all_build_gn_files(self):
        """Find all BUILD.gn files in the codebase"""
        build_files = []
        
        for root, dirs, files in os.walk(self.luxxle_root):
            # Skip backup and .git directories
            dirs[:] = [d for d in dirs if not d.startswith('.') and d != 'backup_before_cleanup']
            
            for file in files:
                if file == 'BUILD.gn':
                    build_file = Path(root) / file
                    build_files.append(build_file)
        
        return build_files
    
    def find_sources_gni_files(self):
        """Find all sources.gni files that might contain luxxle references"""
        sources_files = []
        
        for root, dirs, files in os.walk(self.luxxle_root):
            dirs[:] = [d for d in dirs if not d.startswith('.') and d != 'backup_before_cleanup']
            
            for file in files:
                if file.endswith('sources.gni'):
                    sources_file = Path(root) / file
                    sources_files.append(sources_file)
        
        return sources_files
    
    def backup_file(self, file_path):
        """Create a backup of the file before modification"""
        backup_root = self.luxxle_root / "backup_before_cleanup" / "build_files"
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
    
    def clean_build_file(self, build_file):
        """Clean a single BUILD.gn or sources.gni file of luxxle references"""
        print(f"🔧 Processing: {build_file.relative_to(self.luxxle_root)}")
        
        try:
            with open(build_file, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            print(f"  ✗ Failed to read file: {e}")
            return False
        
        original_content = content
        modifications_made = []
        
        # Patterns to remove or modify
        patterns_to_remove = [
            # Remove luxxle utility importer dependencies
            r'^\s*luxxle_utility_sources\s*\+=\s*luxxle_utility_importer_sources.*$',
            r'^\s*luxxle_utility_deps\s*\+=\s*luxxle_utility_importer_deps.*$', 
            r'^\s*luxxle_utility_public_deps\s*\+=\s*luxxle_utility_importer_public_deps.*$',
            
            # Remove luxxle target dependencies
            r'^\s*"//luxxle/[^"]*".*$',
            r'^\s*deps\s*=\s*\[\s*"//luxxle/[^"]*"\s*\].*$',
            
            # Remove luxxle sources
            r'^\s*sources\s*\+=?\s*luxxle_.*_sources.*$',
            r'^\s*deps\s*\+=?\s*luxxle_.*_deps.*$',
            r'^\s*public_deps\s*\+=?\s*luxxle_.*_public_deps.*$',
            
            # Remove luxxle conditional blocks
            r'^\s*if\s*\(\s*luxxle_.*\s*\)\s*\{[\s\S]*?\}',
            
            # Remove luxxle import statements
            r'^\s*import\("//luxxle/[^"]*"\).*$',
        ]
        
        # String replacements
        replacements = [
            # Replace luxxle references with luxxle
            (r'\bluxxle_utility_', 'luxxle_utility_'),
            (r'\bluxxle_browser_', 'luxxle_browser_'),
            (r'\bluxxle_common_', 'luxxle_common_'),
            (r'"//luxxle/', '"//luxxle/'),
            
            # Update import paths
            (r'import\("//luxxle/', 'import("//luxxle/'),
            
            # Remove luxxle-specific build flags
            (r'luxxle_[a-zA-Z_]*_enabled\s*=\s*[^,\n]*[,\n]?', ''),
        ]
        
        # Apply removals
        lines = content.split('\n')
        new_lines = []
        
        i = 0
        while i < len(lines):
            line = lines[i]
            line_removed = False
            
            # Check if this line should be removed
            for pattern in patterns_to_remove:
                if re.match(pattern, line, re.MULTILINE):
                    modifications_made.append(f"Line {i+1}: Removed '{line.strip()}'")
                    line_removed = True
                    break
            
            # Special handling for luxxle conditional blocks
            if re.match(r'^\s*if\s*\(\s*luxxle_.*\s*\)\s*\{', line):
                # Find the matching closing brace
                brace_count = line.count('{') - line.count('}')
                start_line = i
                
                while i < len(lines) - 1 and brace_count > 0:
                    i += 1
                    brace_count += lines[i].count('{') - lines[i].count('}')
                
                # Remove the entire block
                modifications_made.append(f"Lines {start_line+1}-{i+1}: Removed luxxle conditional block")
                line_removed = True
            
            if not line_removed:
                new_lines.append(line)
            
            i += 1
        
        content = '\n'.join(new_lines)
        
        # Apply string replacements
        for pattern, replacement in replacements:
            new_content = re.sub(pattern, replacement, content, flags=re.MULTILINE)
            if new_content != content:
                modifications_made.append(f"Replaced pattern: {pattern}")
                content = new_content
        
        # Clean up empty dependency arrays and malformed syntax
        cleanup_patterns = [
            # Remove empty dependency arrays
            (r'deps\s*=\s*\[\s*\]', ''),
            (r'sources\s*=\s*\[\s*\]', ''),
            (r'public_deps\s*=\s*\[\s*\]', ''),
            
            # Fix trailing commas in arrays
            (r',(\s*\])', r'\1'),
            
            # Remove multiple empty lines
            (r'\n\s*\n\s*\n', '\n\n'),
        ]
        
        for pattern, replacement in cleanup_patterns:
            content = re.sub(pattern, replacement, content, flags=re.MULTILINE | re.DOTALL)
        
        # Check if modifications were made
        if content != original_content:
            # Backup original file
            if not self.backup_file(build_file):
                print(f"  ⚠️  Backup failed, skipping modifications")
                return False
            
            # Write modified content
            try:
                with open(build_file, 'w', encoding='utf-8') as f:
                    f.write(content)
                
                print(f"  ✓ Modified file with {len(modifications_made)} changes:")
                for mod in modifications_made[:5]:  # Show first 5 changes
                    print(f"    • {mod}")
                if len(modifications_made) > 5:
                    print(f"    • ... and {len(modifications_made) - 5} more changes")
                
                self.modified_files.append(build_file)
                return True
                
            except Exception as e:
                print(f"  ✗ Failed to write file: {e}")
                return False
        else:
            print(f"  ✓ No luxxle references found")
            return True
    
    def verify_build_files(self):
        """Verify that no problematic luxxle references remain"""
        print("\n🔍 Verifying no problematic luxxle references remain...")
        
        all_files = self.find_all_build_gn_files() + self.find_sources_gni_files()
        remaining_refs = []
        
        for build_file in all_files:
            try:
                with open(build_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # Check for problematic luxxle references
                problematic_patterns = [
                    r'"//luxxle/',
                    r'import\("//luxxle/',
                    r'luxxle_utility_importer',
                    r'luxxle_.*_sources',
                    r'luxxle_.*_deps',
                ]
                
                for i, line in enumerate(content.split('\n')):
                    for pattern in problematic_patterns:
                        if re.search(pattern, line):
                            remaining_refs.append({
                                'file': build_file.relative_to(self.luxxle_root),
                                'line': i + 1,
                                'content': line.strip()
                            })
                            
            except Exception as e:
                print(f"  ✗ Failed to verify {build_file}: {e}")
        
        if remaining_refs:
            print(f"⚠️  Found {len(remaining_refs)} remaining problematic luxxle references:")
            for ref in remaining_refs[:10]:  # Show first 10
                print(f"  • {ref['file']}:{ref['line']} - {ref['content']}")
            if len(remaining_refs) > 10:
                print(f"  • ... and {len(remaining_refs) - 10} more references")
            return False
        else:
            print("✅ No problematic luxxle references found in build files!")
            return True
    
    def clean_all_build_files(self):
        """Main method to clean all BUILD.gn and sources.gni files"""
        print("🧹 Starting BUILD.gn File Cleanup - Phase 1.3")
        print(f"Working directory: {self.luxxle_root}")
        print("=" * 60)
        
        # Find all build files
        build_files = self.find_all_build_gn_files()
        sources_files = self.find_sources_gni_files()
        all_files = build_files + sources_files
        
        print(f"Found {len(build_files)} BUILD.gn files and {len(sources_files)} sources.gni files:")
        print(f"Total files to process: {len(all_files)}")
        
        if not all_files:
            print("No build files found.")
            return
        
        response = input(f"\nProceed with cleaning {len(all_files)} build files? (y/N): ")
        if response.lower() != 'y':
            print("Cleanup cancelled.")
            return
        
        print("\n🔧 Processing build files...")
        
        # Process each file
        for build_file in all_files:
            self.clean_build_file(build_file)
            self.processed_files.append(build_file)
        
        # Verify cleanup
        self.verify_build_files()
        
        # Summary
        print("\n" + "=" * 60)
        print("📊 CLEANUP SUMMARY")
        print(f"✓ Files processed: {len(self.processed_files)}")
        print(f"✓ Files modified: {len(self.modified_files)}")
        
        if self.modified_files:
            print("\nModified files:")
            for file_path in self.modified_files[:10]:  # Show first 10
                print(f"  ✓ {file_path.relative_to(self.luxxle_root)}")
            if len(self.modified_files) > 10:
                print(f"  ✓ ... and {len(self.modified_files) - 10} more files")
        
        print(f"\n💾 Backups saved to: {self.luxxle_root}/backup_before_cleanup/build_files/")
        print("✅ Phase 1.3 - BUILD.gn File Cleanup Complete!")


def main():
    # Get the luxxle root directory (current working directory)
    luxxle_root = Path.cwd()
    
    # Verify we're in the right directory
    if not (luxxle_root / "BUILD.gn").exists() or not (luxxle_root / "browser").exists():
        print("❌ Error: This doesn't appear to be the Luxxle root directory.")
        print("Please run this script from the Luxxle source root (where BUILD.gn is located).")
        return
    
    cleaner = BuildGnCleaner(luxxle_root)
    cleaner.clean_all_build_files()


if __name__ == "__main__":
    main() 
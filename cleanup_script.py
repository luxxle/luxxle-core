#!/usr/bin/env python3
"""
Luxxle Brave Component Cleanup Script

This script removes Brave-specific components from the Luxxle codebase
while preserving the essential patch system and build infrastructure.
"""

import os
import shutil
import re
import glob
from pathlib import Path

class LuxxleCleanup:
    def __init__(self, root_dir="."):
        self.root_dir = Path(root_dir)
        self.removed_dirs = []
        self.removed_files = []
        self.preserved_patches = []
        
    def log(self, message):
        print(f"[CLEANUP] {message}")
        
    def remove_brave_components(self):
        """Remove major Brave component directories"""
        brave_components = [
            'components/brave_vpn',
            'components/brave_wallet', 
            'components/brave_webtorrent',
            'components/brave_wayback_machine',
            'components/playlist',
            'components/speedreader',
            'components/tor',
            'components/web_discovery',
            'components/ai_rewriter',
            'components/ntp_background_images',
            'components/request_otr'
        ]
        
        for component in brave_components:
            component_path = self.root_dir / component
            if component_path.exists():
                self.log(f"Removing {component}")
                shutil.rmtree(component_path)
                self.removed_dirs.append(str(component_path))
            else:
                self.log(f"Component {component} not found, skipping")
                
    def remove_brave_browser_features(self):
        """Remove Brave-specific browser features"""
        brave_browser_dirs = [
            'browser/brave_vpn',
            'browser/brave_wallet',
            'browser/brave_stats', 
            'browser/brave_shields',
            'browser/brave_search',
            'browser/speedreader',
            'browser/playlist',
            'browser/tor',
            'browser/p3a',
            'browser/new_tab',
            'browser/ntp_background',
            'browser/decentralized_dns',
            'browser/cosmetic_filters'
        ]
        
        for browser_dir in brave_browser_dirs:
            dir_path = self.root_dir / browser_dir
            if dir_path.exists():
                self.log(f"Removing browser feature: {browser_dir}")
                shutil.rmtree(dir_path)
                self.removed_dirs.append(str(dir_path))
                
    def clean_build_files(self):
        """Remove Brave-specific build files"""
        build_files_to_remove = [
            'brave_paks.gni',
            'brave_repack_locales.gni'
        ]
        
        for build_file in build_files_to_remove:
            file_path = self.root_dir / build_file
            if file_path.exists():
                self.log(f"Removing build file: {build_file}")
                file_path.unlink()
                self.removed_files.append(str(file_path))
                
    def clean_brave_strings(self):
        """Remove Brave-specific string files"""
        string_files = [
            'app/brave_strings.grd',
            'app/brave_generated_resources.grd', 
            'app/brave_settings_strings.grdp',
            'app/settings_brave_strings.grdp',
            'app/brave_command_ids.h'
        ]
        
        for string_file in string_files:
            file_path = self.root_dir / string_file
            if file_path.exists():
                self.log(f"Removing string file: {string_file}")
                file_path.unlink()
                self.removed_files.append(str(file_path))
                
    def clean_source_files(self):
        """Clean Brave references from source files"""
        source_patterns = [
            'sources.gni',
            'BUILD.gn'
        ]
        
        for pattern in source_patterns:
            for file_path in self.root_dir.rglob(pattern):
                if self._should_clean_file(file_path):
                    self.log(f"Cleaning Brave references from: {file_path}")
                    self._remove_brave_refs_from_file(file_path)
                    
    def _should_clean_file(self, file_path):
        """Check if file should be cleaned (not in excluded directories)"""
        excluded_dirs = ['patches', '.git', 'win_build_output', 'node_modules']
        return not any(excluded in str(file_path) for excluded in excluded_dirs)
        
    def _remove_brave_refs_from_file(self, file_path):
        """Remove Brave references from a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                
            # Remove lines containing Brave components
            lines = content.split('\n')
            cleaned_lines = []
            
            for line in lines:
                # Skip lines that reference Brave components we're removing
                if any(brave_ref in line for brave_ref in [
                    'brave_vpn', 'brave_wallet', 'brave_webtorrent',
                    'brave_wayback_machine', 'brave_shields', 'brave_stats',
                    'brave_search', 'speedreader', 'playlist', 'p3a'
                ]):
                    self.log(f"  Removing line: {line.strip()}")
                    continue
                cleaned_lines.append(line)
                
            # Write cleaned content back
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write('\n'.join(cleaned_lines))
                
        except Exception as e:
            self.log(f"Error cleaning {file_path}: {e}")
            
    def preserve_essential_patches(self):
        """Identify and preserve essential patches"""
        patches_dir = self.root_dir / 'patches'
        if not patches_dir.exists():
            return
            
        essential_patterns = [
            'chrome-',  # Chrome modifications
            'chromium-', # Chromium modifications  
            'build-',   # Build system patches
            'third_party-', # Third party patches
            'ui-',      # Essential UI patches
            'content-', # Content process patches
            'components-', # Essential component patches
        ]
        
        for patch_file in patches_dir.glob('*.patch'):
            if any(pattern in patch_file.name for pattern in essential_patterns):
                self.preserved_patches.append(str(patch_file))
                self.log(f"Preserving essential patch: {patch_file.name}")
                
    def update_deps_file(self):
        """Update DEPS file to remove Brave-specific dependencies"""
        deps_file = self.root_dir / 'DEPS'
        if not deps_file.exists():
            return
            
        self.log("Cleaning DEPS file")
        with open(deps_file, 'r') as f:
            content = f.read()
            
        # Remove Brave-specific dependencies
        lines = content.split('\n')
        cleaned_lines = []
        
        for line in lines:
            # Keep essential dependencies, remove Brave-specific ones
            if any(brave_dep in line for brave_dep in [
                'bat-native', 'brave-intl', 'playlist-component'
            ]):
                self.log(f"  Removing dependency: {line.strip()}")
                continue
            cleaned_lines.append(line)
            
        with open(deps_file, 'w') as f:
            f.write('\n'.join(cleaned_lines))
            
    def generate_summary(self):
        """Generate cleanup summary"""
        summary_file = self.root_dir / 'cleanup_summary.txt'
        
        with open(summary_file, 'w') as f:
            f.write("Luxxle Brave Component Cleanup Summary\n")
            f.write("=" * 40 + "\n\n")
            
            f.write(f"Removed Directories ({len(self.removed_dirs)}):\n")
            for dir_path in self.removed_dirs:
                f.write(f"  - {dir_path}\n")
                
            f.write(f"\nRemoved Files ({len(self.removed_files)}):\n")
            for file_path in self.removed_files:
                f.write(f"  - {file_path}\n")
                
            f.write(f"\nPreserved Essential Patches ({len(self.preserved_patches)}):\n")
            for patch in self.preserved_patches:
                f.write(f"  - {patch}\n")
                
        self.log(f"Cleanup summary saved to: {summary_file}")
        
    def run_cleanup(self):
        """Run the complete cleanup process"""
        self.log("Starting Luxxle Brave component cleanup...")
        
        # Create backup branch suggestion
        self.log("IMPORTANT: Create a backup git branch before proceeding!")
        self.log("Run: git checkout -b backup-before-cleanup")
        
        input("Press Enter to continue with cleanup...")
        
        # Phase 1: Component removal
        self.log("Phase 1: Removing Brave components...")
        self.remove_brave_components()
        self.remove_brave_browser_features()
        
        # Phase 2: Build system cleanup
        self.log("Phase 2: Cleaning build system...")
        self.clean_build_files()
        self.clean_brave_strings()
        
        # Phase 3: Source file cleanup
        self.log("Phase 3: Cleaning source files...")
        self.clean_source_files()
        
        # Phase 4: Dependencies cleanup
        self.log("Phase 4: Updating dependencies...")
        self.update_deps_file()
        
        # Phase 5: Preserve patches
        self.log("Phase 5: Preserving essential patches...")
        self.preserve_essential_patches()
        
        # Generate summary
        self.generate_summary()
        
        self.log("Cleanup complete! Check cleanup_summary.txt for details.")
        self.log("Next steps:")
        self.log("1. Test compilation: ninja -C out/Release chrome")
        self.log("2. Verify patches still apply")
        self.log("3. Update any remaining Brave references manually")

if __name__ == "__main__":
    cleanup = LuxxleCleanup()
    cleanup.run_cleanup()

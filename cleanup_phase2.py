#!/usr/bin/env python3
"""
Luxxle Phase 2 Cleanup Script
Removes remaining Brave components while preserving essential Chromium functionality
"""

import os
import shutil
import re
import glob
from pathlib import Path

class LuxxleCleanup:
    def __init__(self, root_dir):
        self.root_dir = Path(root_dir)
        self.removed_files = []
        self.removed_dirs = []
        self.modified_files = []
        
    def log_removal(self, path, is_dir=False):
        """Log what we're removing"""
        if is_dir:
            self.removed_dirs.append(str(path))
            print(f"🗂️  Removing directory: {path}")
        else:
            self.removed_files.append(str(path))
            print(f"🗑️  Removing file: {path}")
    
    def log_modification(self, path):
        """Log what we're modifying"""
        self.modified_files.append(str(path))
        print(f"✏️  Modifying file: {path}")
    
    def safe_remove_file(self, file_path):
        """Safely remove a file if it exists"""
        path = self.root_dir / file_path
        if path.exists():
            self.log_removal(path)
            path.unlink()
            return True
        return False
    
    def safe_remove_dir(self, dir_path):
        """Safely remove a directory if it exists"""
        path = self.root_dir / dir_path
        if path.exists() and path.is_dir():
            self.log_removal(path, is_dir=True)
            shutil.rmtree(path)
            return True
        return False
    
    def remove_brave_browser_files(self):
        """Remove Brave-specific files from browser/ directory"""
        print("\n🧹 Phase 1: Removing Brave browser files...")
        
        brave_browser_files = [
            "browser/brave_content_browser_client.cc",
            "browser/brave_content_browser_client.h",
            "browser/brave_content_browser_client_unittest.cc",
            "browser/brave_content_browser_client_browsertest.cc",
            "browser/brave_browser_process_impl.cc",
            "browser/brave_browser_process_impl.h",
            "browser/brave_browser_process.cc",
            "browser/brave_browser_process.h",
            "browser/brave_profile_prefs.cc",
            "browser/brave_profile_prefs.h",
            "browser/brave_local_state_prefs.cc",
            "browser/brave_local_state_prefs.h",
            "browser/brave_tab_helpers.cc",
            "browser/brave_tab_helpers.h",
            "browser/brave_shell_integration.cc",
            "browser/brave_shell_integration.h",
            "browser/brave_shell_integration_mac.mm",
            "browser/brave_shell_integration_mac.h",
            "browser/brave_shell_integration_win.cc",
            "browser/brave_shell_integration_win.h",
            "browser/brave_features_internal_names.h",
            "browser/brave_drm_tab_helper.cc",
            "browser/brave_drm_tab_helper.h",
            "browser/brave_prefs_browsertest.cc",
            "browser/brave_resources_browsertest.cc",
            "browser/brave_resources_util_unittest.cc",
            "browser/brave_scheme_load_browsertest.cc",
            "browser/browser_context_keyed_service_factories.cc",
            "browser/browser_context_keyed_service_factories.h",
        ]
        
        for file_path in brave_browser_files:
            self.safe_remove_file(file_path)
    
    def remove_brave_directories(self):
        """Remove Brave-specific directories"""
        print("\n🧹 Phase 2: Removing Brave directories...")
        
        brave_dirs = [
            "browser/farbling",
            "browser/skus", 
            "browser/psst",
            "third_party/blink/renderer/modules/brave",
            "third_party/blink/renderer/core/farbling",
            "third_party/blink/renderer/core/brave_page_graph",
        ]
        
        for dir_path in brave_dirs:
            self.safe_remove_dir(dir_path)
    
    def remove_brave_third_party_files(self):
        """Remove Brave-specific third-party modifications"""
        print("\n🧹 Phase 3: Removing Brave third-party files...")
        
        brave_third_party_files = [
            "third_party/blink/renderer/platform/brave_audio_farbling_helper.cc",
            "third_party/blink/renderer/platform/brave_audio_farbling_helper.h",
            "third_party/blink/renderer/brave_farbling_constants.h",
            "v8/include/v8-isolate-page-graph-utils.h",
        ]
        
        for file_path in brave_third_party_files:
            self.safe_remove_file(file_path)
    
    def clean_build_files(self):
        """Clean Brave references from build files"""
        print("\n🧹 Phase 4: Cleaning build files...")
        
        # Files to modify (remove Brave references)
        build_files_to_clean = [
            "BUILD.gn",
            "browser/BUILD.gn",
            "browser/sources.gni",
            "utility/sources.gni",
            "utility/importer/sources.gni",
        ]
        
        for file_path in build_files_to_clean:
            full_path = self.root_dir / file_path
            if full_path.exists():
                self.clean_brave_references_from_file(full_path)
    
    def clean_brave_references_from_file(self, file_path):
        """Remove Brave references from a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            
            # Remove lines containing brave-specific imports or dependencies
            lines = content.split('\n')
            cleaned_lines = []
            
            for line in lines:
                # Skip lines that reference brave components (but keep comments for context)
                if any(pattern in line.lower() for pattern in [
                    'brave_vpn', 'brave_wallet', 'brave_shields', 'brave_ads',
                    'brave_rewards', 'brave_sync', 'brave_search', 'brave_news',
                    'brave_extension', 'brave_webtorrent', 'speedreader',
                    'brave_page_graph', 'brave_farbling', 'brave_skus'
                ]) and not line.strip().startswith('#'):
                    continue
                
                cleaned_lines.append(line)
            
            content = '\n'.join(cleaned_lines)
            
            # Only write if content changed
            if content != original_content:
                self.log_modification(file_path)
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                    
        except Exception as e:
            print(f"❌ Error cleaning {file_path}: {e}")
    
    def update_app_branding(self):
        """Update app branding from Brave to Luxxle"""
        print("\n🎨 Phase 5: Updating branding...")
        
        # Files that need branding updates
        branding_files = [
            "app/brave_main_delegate.cc",
            "app/brave_main_delegate.h",
            "app/brave_exe.rc",
        ]
        
        for file_path in branding_files:
            full_path = self.root_dir / file_path
            if full_path.exists():
                self.update_branding_in_file(full_path)
    
    def update_branding_in_file(self, file_path):
        """Replace Brave branding with Luxxle in a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            
            # Replace Brave with Luxxle (preserve case)
            content = re.sub(r'\bBrave\b', 'Luxxle', content)
            content = re.sub(r'\bbrave\b', 'luxxle', content)
            content = re.sub(r'\bBRAVE\b', 'LUXXLE', content)
            
            # Only write if content changed
            if content != original_content:
                self.log_modification(file_path)
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                    
        except Exception as e:
            print(f"❌ Error updating branding in {file_path}: {e}")
    
    def generate_cleanup_report(self):
        """Generate a report of what was cleaned up"""
        report_path = self.root_dir / "cleanup_phase2_report.txt"
        
        with open(report_path, 'w') as f:
            f.write("Luxxle Phase 2 Cleanup Report\n")
            f.write("==============================\n\n")
            
            f.write(f"Removed Files ({len(self.removed_files)}):\n")
            for file_path in sorted(self.removed_files):
                f.write(f"  - {file_path}\n")
            
            f.write(f"\nRemoved Directories ({len(self.removed_dirs)}):\n")
            for dir_path in sorted(self.removed_dirs):
                f.write(f"  - {dir_path}\n")
            
            f.write(f"\nModified Files ({len(self.modified_files)}):\n")
            for file_path in sorted(self.modified_files):
                f.write(f"  - {file_path}\n")
        
        print(f"\n📊 Cleanup report saved to: {report_path}")
    
    def run_cleanup(self):
        """Run the complete cleanup process"""
        print("🚀 Starting Luxxle Phase 2 Cleanup...")
        print(f"Working directory: {self.root_dir}")
        
        self.remove_brave_browser_files()
        self.remove_brave_directories()
        self.remove_brave_third_party_files()
        self.clean_build_files()
        self.update_app_branding()
        
        self.generate_cleanup_report()
        
        print("\n✅ Phase 2 cleanup completed!")
        print(f"📈 Summary:")
        print(f"  - Removed {len(self.removed_files)} files")
        print(f"  - Removed {len(self.removed_dirs)} directories")
        print(f"  - Modified {len(self.modified_files)} files")

if __name__ == "__main__":
    # Run cleanup from current directory
    cleanup = LuxxleCleanup(".")
    cleanup.run_cleanup() 
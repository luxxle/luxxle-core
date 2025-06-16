#!/usr/bin/env python3
"""
Final Luxxle Cleanup Script
Complete removal of Brave components while preserving essential Chromium functionality
and the ability to apply patches like Brave does.
"""

import os
import shutil
import re
import subprocess
from pathlib import Path
import json
from datetime import datetime

class FinalLuxxleCleanup:
    def __init__(self, root_dir):
        self.root_dir = Path(root_dir)
        self.backup_dir = self.root_dir / "backup_before_cleanup"
        self.cleanup_log = []
        
        # Critical files that should NEVER be removed
        self.protected_files = {
            'BUILD.gn',
            'DEPS', 
            'patches/',
            'chromium_src/',
            '.gclient',
            '.git/',
        }
        
    def log_action(self, action, path, status="SUCCESS"):
        """Log cleanup actions"""
        entry = {
            'timestamp': datetime.now().isoformat(),
            'action': action,
            'path': str(path),
            'status': status
        }
        self.cleanup_log.append(entry)
        status_emoji = "✅" if status == "SUCCESS" else "❌" if status == "ERROR" else "⚠️"
        print(f"{status_emoji} {action}: {path}")
        
    def create_backup(self):
        """Create backup of critical files before cleanup"""
        print("📦 Creating backup of critical files...")
        
        if self.backup_dir.exists():
            shutil.rmtree(self.backup_dir)
        self.backup_dir.mkdir()
        
        # Backup critical configuration files
        critical_files = [
            'BUILD.gn',
            'browser/BUILD.gn', 
            'app/brave_main_delegate.cc',
            'app/brave_main_delegate.h',
            'DEPS'
        ]
        
        for file_path in critical_files:
            src = self.root_dir / file_path
            if src.exists():
                dst = self.backup_dir / file_path
                dst.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(src, dst)
                self.log_action("BACKUP", file_path)
    
    def remove_brave_browser_core(self):
        """Remove core Brave browser implementation files"""
        print("\n🗑️ Phase 1: Removing Brave core browser files...")
        
        brave_core_files = [
            # Core browser client
            "browser/brave_content_browser_client.cc",
            "browser/brave_content_browser_client.h",
            "browser/brave_content_browser_client_unittest.cc",
            "browser/brave_content_browser_client_browsertest.cc",
            
            # Browser process
            "browser/brave_browser_process_impl.cc",
            "browser/brave_browser_process_impl.h", 
            "browser/brave_browser_process.cc",
            "browser/brave_browser_process.h",
            
            # Preferences
            "browser/brave_profile_prefs.cc",
            "browser/brave_profile_prefs.h",
            "browser/brave_local_state_prefs.cc", 
            "browser/brave_local_state_prefs.h",
            
            # Tab helpers
            "browser/brave_tab_helpers.cc",
            "browser/brave_tab_helpers.h",
            
            # Shell integration
            "browser/brave_shell_integration.cc",
            "browser/brave_shell_integration.h",
            "browser/brave_shell_integration_mac.mm",
            "browser/brave_shell_integration_mac.h", 
            "browser/brave_shell_integration_win.cc",
            "browser/brave_shell_integration_win.h",
            
            # Features and utilities
            "browser/brave_features_internal_names.h",
            "browser/brave_drm_tab_helper.cc",
            "browser/brave_drm_tab_helper.h",
            "browser/default_protocol_handler_utils_win.cc",
            "browser/default_protocol_handler_utils_win.h",
            "browser/default_protocol_handler_utils_win_unittest.cc",
            
            # Service factories
            "browser/browser_context_keyed_service_factories.cc",
            "browser/browser_context_keyed_service_factories.h",
            
            # Mac specific
            "browser/mac_features.cc",
            "browser/mac_features.h",
            "browser/brave_app_controller_mac.mm",
        ]
        
        for file_path in brave_core_files:
            self.safe_remove_file(file_path)
    
    def remove_brave_test_files(self):
        """Remove Brave test files"""
        print("\n🧪 Phase 2: Removing Brave test files...")
        
        test_patterns = [
            "browser/brave_*_test.cc",
            "browser/brave_*_unittest.cc", 
            "browser/brave_*_browsertest.cc",
            "browser/brave_resources_*.cc",
            "browser/brave_scheme_load_browsertest.cc",
            "browser/brave_prefs_browsertest.cc",
        ]
        
        for pattern in test_patterns:
            for file_path in self.root_dir.glob(pattern):
                if file_path.is_file():
                    self.safe_remove_file(file_path.relative_to(self.root_dir))
    
    def remove_brave_feature_directories(self):
        """Remove Brave-specific feature directories"""
        print("\n📁 Phase 3: Removing Brave feature directories...")
        
        brave_feature_dirs = [
            # Privacy/Security features (Brave-specific implementations)
            "browser/farbling",
            "browser/ephemeral_storage", 
            
            # Brave services
            "browser/skus",
            "browser/psst",
            
            # Third-party Brave modifications
            "third_party/blink/renderer/modules/brave",
            "third_party/blink/renderer/core/farbling", 
            "third_party/blink/renderer/core/brave_page_graph",
            "third_party/blink/renderer/core/resource_pool_limiter",
            
            # Utility extensions
            "utility/importer",  # Brave-specific import functionality
        ]
        
        for dir_path in brave_feature_dirs:
            self.safe_remove_dir(dir_path)
    
    def remove_brave_third_party_files(self):
        """Remove individual Brave third-party files"""
        print("\n🔧 Phase 4: Removing Brave third-party files...")
        
        brave_third_party_files = [
            "third_party/blink/renderer/platform/brave_audio_farbling_helper.cc",
            "third_party/blink/renderer/platform/brave_audio_farbling_helper.h",
            "third_party/blink/renderer/brave_farbling_constants.h",
            "v8/include/v8-isolate-page-graph-utils.h",
            "third_party/blink/renderer/modules/global_privacy_control/global_privacy_control.h",
            "third_party/blink/renderer/modules/global_privacy_control/global_privacy_control.cc",
        ]
        
        for file_path in brave_third_party_files:
            self.safe_remove_file(file_path)
    
    def clean_build_system(self):
        """Clean Brave references from build system"""
        print("\n🔨 Phase 5: Cleaning build system...")
        
        # Update main BUILD.gn to be minimal Chromium-focused
        self.update_main_build_file()
        
        # Clean browser BUILD.gn 
        self.update_browser_build_file()
        
        # Update sources.gni files
        self.clean_sources_gni_files()
        
        # Clean app BUILD.gn
        self.clean_app_build_file()
    
    def update_main_build_file(self):
        """Update the main BUILD.gn file"""
        build_file = self.root_dir / "BUILD.gn"
        if not build_file.exists():
            return
            
        with open(build_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Simplified Luxxle BUILD.gn focused on Chromium
        new_content = '''# Copyright (c) 2017 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

# LUXXLE: Minimal Chromium-based browser build configuration

import("//build/config/locales.gni")
import("//chrome/browser/buildflags.gni") 
import("//tools/grit/repack.gni")

group("all") {
  testonly = true
  # Build standard Chromium
  deps = [ "//chrome" ]
  
  if (!is_android && !is_ios) {
    deps += [
      "//chrome/test:browser_tests",
      "//chrome/test:unit_tests", 
    ]
  }
}

if (!is_ios) {
  group("child_dependencies") {
    deps = [
      "common",
      "//luxxle/renderer", 
      "//luxxle/utility",
    ]
  }

  group("browser_dependencies") {
    deps = [
      "browser",
      "common",
    ]
    
    if (!is_ios) {
      deps += [ ":packed_resources" ]
    }
  }
}

group("packed_resources") {
  deps = [ "//chrome:packed_resources" ]
}

group("luxxle") {
  deps = [ "//chrome" ]
}

group("resources") {
  public_deps = []
}

if (is_mac) {
  group("framework_bundle_data") {
    deps = []
  }
}
'''
        
        with open(build_file, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        self.log_action("UPDATE", "BUILD.gn")
    
    def update_browser_build_file(self):
        """Update browser/BUILD.gn to remove Brave dependencies"""
        build_file = self.root_dir / "browser/BUILD.gn"
        if not build_file.exists():
            return
            
        with open(build_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Remove lines referencing Brave components
        lines = content.split('\n')
        cleaned_lines = []
        
        skip_patterns = [
            'brave_', 'farbling', 'skus', 'psst', 'page_graph',
            'shields', 'rewards', 'vpn', 'wallet', 'ads'
        ]
        
        for line in lines:
            if not any(pattern in line.lower() for pattern in skip_patterns) or line.strip().startswith('#'):
                cleaned_lines.append(line)
        
        cleaned_content = '\n'.join(cleaned_lines)
        
        with open(build_file, 'w', encoding='utf-8') as f:
            f.write(cleaned_content)
        
        self.log_action("UPDATE", "browser/BUILD.gn")
    
    def clean_sources_gni_files(self):
        """Clean .gni files of Brave references"""
        gni_files = [
            "browser/sources.gni",
            "utility/sources.gni", 
            "utility/importer/sources.gni",
            "v8/sources.gni",
        ]
        
        for gni_path in gni_files:
            gni_file = self.root_dir / gni_path
            if gni_file.exists():
                self.clean_brave_references_from_file(gni_file)
    
    def clean_app_build_file(self):
        """Clean app/BUILD.gn"""
        build_file = self.root_dir / "app/BUILD.gn"
        if build_file.exists():
            self.clean_brave_references_from_file(build_file)
    
    def update_branding(self):
        """Update branding from Brave to Luxxle"""
        print("\n🎨 Phase 6: Updating branding...")
        
        branding_files = [
            "app/brave_main_delegate.cc",
            "app/brave_main_delegate.h",
            "app/brave_exe.rc",
        ]
        
        for file_path in branding_files:
            full_path = self.root_dir / file_path
            if full_path.exists():
                self.update_branding_in_file(full_path)
    
    def remove_brave_patches(self):
        """Remove Brave-specific patches using our audit results"""
        print("\n🧹 Phase 7: Removing Brave-specific patches...")
        
        # Use the generated removal script
        script_path = self.root_dir / "remove_brave_patches.py"
        if script_path.exists():
            try:
                subprocess.run(['python', str(script_path)], 
                             cwd=self.root_dir, check=True)
                self.log_action("EXECUTE", "remove_brave_patches.py")
            except subprocess.CalledProcessError as e:
                self.log_action("EXECUTE", "remove_brave_patches.py", "ERROR")
    
    def verify_essential_files(self):
        """Verify essential files still exist"""
        print("\n🔍 Phase 8: Verifying essential files...")
        
        essential_files = [
            "BUILD.gn",
            "DEPS",
            "chromium_src/",
            "patches/",
            ".git/",
        ]
        
        missing_files = []
        for file_path in essential_files:
            full_path = self.root_dir / file_path
            if not full_path.exists():
                missing_files.append(file_path)
                self.log_action("VERIFY", file_path, "MISSING")
            else:
                self.log_action("VERIFY", file_path, "OK")
        
        if missing_files:
            print(f"⚠️ Warning: Missing essential files: {missing_files}")
            return False
        return True
    
    def generate_final_report(self):
        """Generate comprehensive cleanup report"""
        print("\n📊 Generating final cleanup report...")
        
        report_path = self.root_dir / "luxxle_cleanup_final_report.json"
        
        # Count actions by type
        action_counts = {}
        for entry in self.cleanup_log:
            action = entry['action']
            action_counts[action] = action_counts.get(action, 0) + 1
        
        # Generate summary
        report = {
            "cleanup_completed": datetime.now().isoformat(),
            "total_actions": len(self.cleanup_log),
            "action_summary": action_counts,
            "cleanup_log": self.cleanup_log,
            "next_steps": [
                "1. Test compilation with: python script/build.py",
                "2. Verify browser launches correctly",
                "3. Test patch system with a simple patch",
                "4. Remove backup directory if everything works",
            ]
        }
        
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2)
        
        self.log_action("GENERATE", "luxxle_cleanup_final_report.json")
        
        # Also generate human-readable summary
        summary_path = self.root_dir / "cleanup_summary_final.txt"
        with open(summary_path, 'w', encoding='utf-8') as f:
            f.write("Luxxle Final Cleanup Summary\n")
            f.write("============================\n\n")
            f.write(f"Cleanup completed: {datetime.now().isoformat()}\n")
            f.write(f"Total actions performed: {len(self.cleanup_log)}\n\n")
            
            f.write("Actions by type:\n")
            for action, count in action_counts.items():
                f.write(f"  {action}: {count}\n")
            
            f.write("\nNext steps:\n")
            f.write("1. Test compilation: python script/build.py\n")
            f.write("2. Verify browser launches\n") 
            f.write("3. Test applying a simple patch\n")
            f.write("4. Remove backup if everything works\n")
        
        return report_path
    
    # Helper methods
    def safe_remove_file(self, file_path):
        """Safely remove a file"""
        path = self.root_dir / file_path if isinstance(file_path, str) else file_path
        if path.exists():
            path.unlink()
            self.log_action("REMOVE_FILE", file_path)
            return True
        return False
    
    def safe_remove_dir(self, dir_path):
        """Safely remove a directory"""
        path = self.root_dir / dir_path if isinstance(dir_path, str) else dir_path
        if path.exists() and path.is_dir():
            shutil.rmtree(path)
            self.log_action("REMOVE_DIR", dir_path)
            return True
        return False
    
    def clean_brave_references_from_file(self, file_path):
        """Remove Brave references from a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            lines = content.split('\n')
            cleaned_lines = []
            
            skip_patterns = [
                'brave_vpn', 'brave_wallet', 'brave_shields', 'brave_ads',
                'brave_rewards', 'brave_sync', 'brave_search', 'brave_news',
                'brave_extension', 'brave_webtorrent', 'speedreader',
                'brave_page_graph', 'brave_farbling', 'brave_skus', 'farbling',
                'page_graph'
            ]
            
            for line in lines:
                if not any(pattern in line.lower() for pattern in skip_patterns) or line.strip().startswith('#'):
                    cleaned_lines.append(line)
            
            content = '\n'.join(cleaned_lines)
            
            if content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                self.log_action("CLEAN", file_path)
                
        except Exception as e:
            self.log_action("CLEAN", file_path, "ERROR")
    
    def update_branding_in_file(self, file_path):
        """Update branding in a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            original_content = content
            
            # Replace Brave with Luxxle
            content = re.sub(r'\bBrave\b', 'Luxxle', content)
            content = re.sub(r'\bbrave\b', 'luxxle', content) 
            content = re.sub(r'\bBRAVE\b', 'LUXXLE', content)
            
            if content != original_content:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                self.log_action("REBRAND", file_path)
                
        except Exception as e:
            self.log_action("REBRAND", file_path, "ERROR")
    
    def run_complete_cleanup(self):
        """Run the complete cleanup process"""
        print("🚀 Starting Final Luxxle Cleanup Process...")
        print(f"Working directory: {self.root_dir}")
        print("This will transform your Brave fork into a clean Chromium-based browser")
        print("while preserving the ability to apply patches.\n")
        
        try:
            self.create_backup()
            self.remove_brave_browser_core()
            self.remove_brave_test_files() 
            self.remove_brave_feature_directories()
            self.remove_brave_third_party_files()
            self.clean_build_system()
            self.update_branding()
            self.remove_brave_patches()
            
            if self.verify_essential_files():
                report_path = self.generate_final_report()
                
                print("\n🎉 Luxxle cleanup completed successfully!")
                print(f"📊 Full report: {report_path}")
                print("\n📋 Summary:")
                print(f"  • Performed {len(self.cleanup_log)} cleanup actions")
                print(f"  • Backup created in: {self.backup_dir}")
                print(f"  • Essential files verified: ✅")
                
                print("\n🔄 Next steps:")
                print("1. Test compilation with your build system")
                print("2. Verify the browser launches correctly")
                print("3. Test that the patch system still works")
                print("4. If everything works, you can remove the backup directory")
                
                return True
            else:
                print("\n❌ Cleanup completed but some essential files are missing!")
                print("Please check the verification errors above.")
                return False
                
        except Exception as e:
            print(f"\n❌ Cleanup failed with error: {e}")
            print(f"Backup available in: {self.backup_dir}")
            return False

def main():
    cleanup = FinalLuxxleCleanup(".")
    success = cleanup.run_complete_cleanup()
    return 0 if success else 1

if __name__ == "__main__":
    exit(main()) 
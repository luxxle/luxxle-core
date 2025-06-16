#!/usr/bin/env python3
"""
Brave Directory Cleaner - Phase 1.1 Implementation
Removes Brave-specific directories that will cause build failures
"""

import os
import shutil
import sys
from pathlib import Path

class BraveDirectoryCleaner:
    def __init__(self, luxxle_root):
        self.luxxle_root = Path(luxxle_root)
        self.removed_dirs = []
        self.not_found_dirs = []
        
    def get_brave_directories_to_remove(self):
        """Define the list of Brave-specific directories to remove"""
        return [
            # Browser Brave directories
            "browser/brave_ads",
            "browser/brave_adaptive_captcha", 
            "browser/ai_chat",
            "browser/autoplay",
            "browser/banners",
            
            # iOS Brave implementation
            "ios/brave-ios",
            
            # Additional Brave directories we might find
            "browser/brave_wallet",
            "browser/brave_rewards", 
            "browser/brave_shields",
            "browser/brave_sync",
            "browser/brave_new_tab_ui",
            "browser/brave_welcome",
            "browser/brave_vpn",
            "browser/brave_search",
            "browser/ipfs",
            "browser/tor",
            "browser/speedreader",
            "browser/ethereum_remote_client",
            "browser/brave_stats",
            "browser/brave_today",
            "browser/brave_webtorrent",
            "browser/widevine",
            
            # Components that might be Brave-specific
            "components/brave_ads",
            "components/brave_rewards",
            "components/brave_wallet", 
            "components/brave_shields",
            "components/brave_sync",
            "components/brave_search_conversion",
            "components/brave_perf_predictor",
            "components/brave_referrals",
            "components/brave_component_updater",
            "components/speedreader",
            "components/ipfs",
            "components/tor",
            
            # Services
            "services/brave_wallet",
            "services/bat_ledger",
            "services/bat_ads",
            "services/bat_rewards",
            
            # Tools
            "tools/brave_vpn",
        ]
    
    def backup_directory(self, dir_path):
        """Create a backup of the directory before removal"""
        backup_root = self.luxxle_root / "backup_before_cleanup" / "directories"
        backup_root.mkdir(parents=True, exist_ok=True)
        
        # Create backup path preserving directory structure
        relative_path = dir_path.relative_to(self.luxxle_root)
        backup_path = backup_root / relative_path
        backup_path.parent.mkdir(parents=True, exist_ok=True)
        
        try:
            shutil.copytree(dir_path, backup_path)
            print(f"  ✓ Backed up to: {backup_path}")
            return True
        except Exception as e:
            print(f"  ✗ Backup failed: {e}")
            return False
    
    def remove_directory(self, relative_path):
        """Remove a directory if it exists"""
        dir_path = self.luxxle_root / relative_path
        
        if not dir_path.exists():
            print(f"⚠️  Directory not found: {relative_path}")
            self.not_found_dirs.append(relative_path)
            return False
            
        if not dir_path.is_dir():
            print(f"⚠️  Path is not a directory: {relative_path}")
            return False
            
        print(f"🗂️  Removing directory: {relative_path}")
        
        # Create backup
        if not self.backup_directory(dir_path):
            response = input(f"Backup failed for {relative_path}. Continue anyway? (y/N): ")
            if response.lower() != 'y':
                return False
        
        try:
            shutil.rmtree(dir_path)
            print(f"  ✓ Successfully removed: {relative_path}")
            self.removed_dirs.append(relative_path)
            return True
        except Exception as e:
            print(f"  ✗ Failed to remove {relative_path}: {e}")
            return False
    
    def scan_for_additional_brave_dirs(self):
        """Scan for any additional directories with 'brave' in the name"""
        print("\n🔍 Scanning for additional directories with 'brave' in the name...")
        
        additional_dirs = []
        
        for root, dirs, files in os.walk(self.luxxle_root):
            # Skip backup and .git directories
            dirs[:] = [d for d in dirs if not d.startswith('.') and d != 'backup_before_cleanup']
            
            for dir_name in dirs:
                if 'brave' in dir_name.lower():
                    dir_path = Path(root) / dir_name
                    relative_path = dir_path.relative_to(self.luxxle_root)
                    additional_dirs.append(str(relative_path))
        
        if additional_dirs:
            print(f"Found {len(additional_dirs)} additional directories with 'brave' in name:")
            for dir_path in additional_dirs:
                print(f"  • {dir_path}")
            
            response = input("\nRemove these additional directories? (y/N): ")
            if response.lower() == 'y':
                for dir_path in additional_dirs:
                    self.remove_directory(dir_path)
        else:
            print("No additional 'brave' directories found.")
    
    def clean_brave_directories(self):
        """Main method to clean all Brave directories"""
        print("🧹 Starting Brave Directory Cleanup - Phase 1.1")
        print(f"Working directory: {self.luxxle_root}")
        print("=" * 60)
        
        directories_to_remove = self.get_brave_directories_to_remove()
        
        print(f"Planning to remove {len(directories_to_remove)} directories...")
        
        # Show what will be removed
        print("\nDirectories to remove:")
        for dir_path in directories_to_remove:
            status = "✓ EXISTS" if (self.luxxle_root / dir_path).exists() else "✗ NOT FOUND"
            print(f"  • {dir_path} - {status}")
        
        response = input(f"\nProceed with removal? (y/N): ")
        if response.lower() != 'y':
            print("Cleanup cancelled.")
            return
        
        print("\n🗂️ Starting directory removal...")
        
        # Remove each directory
        for dir_path in directories_to_remove:
            self.remove_directory(dir_path)
        
        # Scan for additional brave directories
        self.scan_for_additional_brave_dirs()
        
        # Summary
        print("\n" + "=" * 60)
        print("📊 CLEANUP SUMMARY")
        print(f"✓ Directories successfully removed: {len(self.removed_dirs)}")
        print(f"⚠️  Directories not found: {len(self.not_found_dirs)}")
        
        if self.removed_dirs:
            print("\nRemoved directories:")
            for dir_path in self.removed_dirs:
                print(f"  ✓ {dir_path}")
        
        if self.not_found_dirs:
            print("\nDirectories not found (already removed?):")
            for dir_path in self.not_found_dirs:
                print(f"  ⚠️  {dir_path}")
        
        print(f"\n💾 Backups saved to: {self.luxxle_root}/backup_before_cleanup/directories/")
        print("✅ Phase 1.1 - Brave Directory Cleanup Complete!")


def main():
    # Get the luxxle root directory (current working directory)
    luxxle_root = Path.cwd()
    
    # Verify we're in the right directory
    if not (luxxle_root / "BUILD.gn").exists() or not (luxxle_root / "browser").exists():
        print("❌ Error: This doesn't appear to be the Luxxle root directory.")
        print("Please run this script from the Luxxle source root (where BUILD.gn is located).")
        sys.exit(1)
    
    cleaner = BraveDirectoryCleaner(luxxle_root)
    cleaner.clean_brave_directories()


if __name__ == "__main__":
    main() 
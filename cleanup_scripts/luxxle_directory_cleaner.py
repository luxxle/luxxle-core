#!/usr/bin/env python3
"""
Luxxle Directory Cleaner - Phase 1.1 Implementation
Removes Luxxle-specific directories that will cause build failures
"""

import os
import shutil
import sys
from pathlib import Path

class LuxxleDirectoryCleaner:
    def __init__(self, luxxle_root):
        self.luxxle_root = Path(luxxle_root)
        self.removed_dirs = []
        self.not_found_dirs = []
        
    def get_luxxle_directories_to_remove(self):
        """Define the list of Luxxle-specific directories to remove"""
        return [
            # Browser Luxxle directories
            "browser/luxxle_ads",
            "browser/luxxle_adaptive_captcha", 
            "browser/ai_chat",
            "browser/autoplay",
            "browser/banners",
            
            # iOS Luxxle implementation
            "ios/luxxle-ios",
            
            # Additional Luxxle directories we might find
            "browser/luxxle_wallet",
            "browser/luxxle_rewards", 
            "browser/luxxle_shields",
            "browser/luxxle_sync",
            "browser/luxxle_new_tab_ui",
            "browser/luxxle_welcome",
            "browser/luxxle_vpn",
            "browser/luxxle_search",
            "browser/ipfs",
            "browser/tor",
            "browser/speedreader",
            "browser/ethereum_remote_client",
            "browser/luxxle_stats",
            "browser/luxxle_today",
            "browser/luxxle_webtorrent",
            "browser/widevine",
            
            # Components that might be Luxxle-specific
            "components/luxxle_ads",
            "components/luxxle_rewards",
            "components/luxxle_wallet", 
            "components/luxxle_shields",
            "components/luxxle_sync",
            "components/luxxle_search_conversion",
            "components/luxxle_perf_predictor",
            "components/luxxle_referrals",
            "components/luxxle_component_updater",
            "components/speedreader",
            "components/ipfs",
            "components/tor",
            
            # Services
            "services/luxxle_wallet",
            "services/bat_ledger",
            "services/bat_ads",
            "services/bat_rewards",
            
            # Tools
            "tools/luxxle_vpn",
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
    
    def scan_for_additional_luxxle_dirs(self):
        """Scan for any additional directories with 'luxxle' in the name"""
        print("\n🔍 Scanning for additional directories with 'luxxle' in the name...")
        
        additional_dirs = []
        
        for root, dirs, files in os.walk(self.luxxle_root):
            # Skip backup and .git directories
            dirs[:] = [d for d in dirs if not d.startswith('.') and d != 'backup_before_cleanup']
            
            for dir_name in dirs:
                if 'luxxle' in dir_name.lower():
                    dir_path = Path(root) / dir_name
                    relative_path = dir_path.relative_to(self.luxxle_root)
                    additional_dirs.append(str(relative_path))
        
        if additional_dirs:
            print(f"Found {len(additional_dirs)} additional directories with 'luxxle' in name:")
            for dir_path in additional_dirs:
                print(f"  • {dir_path}")
            
            response = input("\nRemove these additional directories? (y/N): ")
            if response.lower() == 'y':
                for dir_path in additional_dirs:
                    self.remove_directory(dir_path)
        else:
            print("No additional 'luxxle' directories found.")
    
    def clean_luxxle_directories(self):
        """Main method to clean all Luxxle directories"""
        print("🧹 Starting Luxxle Directory Cleanup - Phase 1.1")
        print(f"Working directory: {self.luxxle_root}")
        print("=" * 60)
        
        directories_to_remove = self.get_luxxle_directories_to_remove()
        
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
        
        # Scan for additional luxxle directories
        self.scan_for_additional_luxxle_dirs()
        
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
        print("✅ Phase 1.1 - Luxxle Directory Cleanup Complete!")


def main():
    # Get the luxxle root directory (current working directory)
    luxxle_root = Path.cwd()
    
    # Verify we're in the right directory
    if not (luxxle_root / "BUILD.gn").exists() or not (luxxle_root / "browser").exists():
        print("❌ Error: This doesn't appear to be the Luxxle root directory.")
        print("Please run this script from the Luxxle source root (where BUILD.gn is located).")
        sys.exit(1)
    
    cleaner = LuxxleDirectoryCleaner(luxxle_root)
    cleaner.clean_luxxle_directories()


if __name__ == "__main__":
    main() 
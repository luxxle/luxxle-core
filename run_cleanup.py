#!/usr/bin/env python3
"""
Master Luxxle Cleanup Script
Executes all cleanup phases in the correct order to strip Luxxle components
"""

import os
import sys
import subprocess
from pathlib import Path

def run_script(script_path, description):
    """Run a cleanup script and handle errors"""
    print(f"\n{'='*60}")
    print(f"🚀 Starting: {description}")
    print(f"{'='*60}")
    
    try:
        result = subprocess.run([sys.executable, script_path], 
                              capture_output=False, 
                              text=True, 
                              cwd=Path.cwd())
        
        if result.returncode == 0:
            print(f"✅ Completed: {description}")
            return True
        else:
            print(f"❌ Failed: {description} (exit code: {result.returncode})")
            return False
            
    except Exception as e:
        print(f"❌ Error running {script_path}: {e}")
        return False

def main():
    print("🧹 LUXXLE CLEANUP - MASTER SCRIPT")
    print("This script will remove all Luxxle components from the Luxxle codebase")
    print("=" * 60)
    
    # Verify we're in the correct directory
    if not (Path.cwd() / "BUILD.gn").exists() or not (Path.cwd() / "browser").exists():
        print("❌ Error: This doesn't appear to be the Luxxle root directory.")
        print("Please run this script from the Luxxle source root (where BUILD.gn is located).")
        sys.exit(1)
    
    # Create cleanup_scripts directory if it doesn't exist
    cleanup_dir = Path.cwd() / "cleanup_scripts"
    cleanup_dir.mkdir(exist_ok=True)
    
    # Phase 1: Critical Build System Cleanup
    print("\n🎯 PHASE 1: CRITICAL BUILD SYSTEM CLEANUP")
    
    # Phase 1.1: Remove Luxxle Directories
    script_1_1 = cleanup_dir / "luxxle_directory_cleaner.py"
    if script_1_1.exists():
        if not run_script(script_1_1, "Phase 1.1 - Remove Luxxle Directories"):
            print("⚠️  Phase 1.1 failed. Continue anyway? (y/N): ", end="")
            if input().lower() != 'y':
                sys.exit(1)
    else:
        print("⚠️  Script not found: luxxle_directory_cleaner.py")
    
    # Phase 1.2: Clean DEPS Files  
    script_1_2 = cleanup_dir / "deps_cleaner.py"
    if script_1_2.exists():
        if not run_script(script_1_2, "Phase 1.2 - Clean DEPS Files"):
            print("⚠️  Phase 1.2 failed. Continue anyway? (y/N): ", end="")
            if input().lower() != 'y':
                sys.exit(1)
    else:
        print("⚠️  Script not found: deps_cleaner.py")
    
    # Manual tasks that need attention
    print("\n" + "="*60)
    print("🔧 MANUAL TASKS REQUIRED")
    print("="*60)
    print("The following tasks need manual attention:")
    print()
    print("1. 📁 Rename Luxxle-named files:")
    print("   - browser/luxxle_browser_main_parts_mac.h → luxxle_browser_main_parts_mac.h")
    print("   - browser/luxxle_browser_main_parts.h → luxxle_browser_main_parts.h") 
    print("   - browser/luxxle_browser_features.h → luxxle_browser_features.h")
    print("   - browser/luxxle_app_controller_mac.h → luxxle_app_controller_mac.h")
    print()
    print("2. 🔧 Update BUILD.gn files:")
    print("   - Remove luxxle utility dependencies in utility/BUILD.gn")
    print("   - Remove luxxle sources in utility/sources.gni")
    print("   - Update browser/BUILD.gn luxxle targets")
    print()
    print("3. 🎨 Update UI Components:")
    print("   - Replace @luxxle/leo imports in ui/webui/resources/")
    print("   - Update package.json dependencies")
    print("   - Fix CSS variables (--luxxle-toolbar-gradient)")
    print()
    print("4. ⚙️  Configuration Updates:")
    print("   - Remove .luxxle_gclient file")
    print("   - Update search engine defaults to luxxle.com")
    print("   - Update branding and metadata")
    print()
    print("5. 🧪 Test Build:")
    print("   - Run build system to check for compilation errors")
    print("   - Fix any remaining dependency issues")
    
    print("\n" + "="*60)
    print("✅ AUTOMATED CLEANUP PHASES COMPLETE!")
    print("✅ Please complete the manual tasks above, then test the build.")
    print("="*60)

if __name__ == "__main__":
    main() 
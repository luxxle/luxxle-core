#!/usr/bin/env python3
"""
Master script to run the complete Luxxle cleanup process.
This will clean up BUILD.gn files, includes, and resources in the proper order.
"""

import os
import sys
import subprocess
import argparse
import shutil
from pathlib import Path

def run_script(script_name, args=None):
    """Run a cleanup script and handle errors."""
    if args is None:
        args = []
    
    script_path = Path(script_name)
    if not script_path.exists():
        print(f"❌ Error: {script_name} not found!")
        return False
    
    cmd = [sys.executable, str(script_path)] + args
    print(f"\n🔧 Running: {' '.join(cmd)}")
    
    try:
        result = subprocess.run(cmd, check=True, capture_output=False)
        print(f"✅ {script_name} completed successfully")
        return True
    except subprocess.CalledProcessError as e:
        print(f"❌ {script_name} failed with exit code {e.returncode}")
        return False

def check_git_status():
    """Check if we're in a git repository and if there are uncommitted changes."""
    try:
        result = subprocess.run(['git', 'status', '--porcelain'], 
                              capture_output=True, text=True, check=True)
        if result.stdout.strip():
            print("⚠️  Warning: You have uncommitted changes in your git repository.")
            print("   It's recommended to commit or stash changes before running cleanup.")
            response = input("   Continue anyway? (y/N): ")
            if response.lower() not in ['y', 'yes']:
                return False
        return True
    except subprocess.CalledProcessError:
        print("⚠️  Warning: Not in a git repository or git not available.")
        print("   Consider using version control for safety.")
        return True

def create_backup():
    """Create a backup of critical files before making changes."""
    print("\n📁 Creating backup of critical files...")
    
    backup_dir = Path("luxxle_cleanup_backup")
    backup_dir.mkdir(exist_ok=True)
    
    critical_files = [
        "BUILD.gn",
        "browser/BUILD.gn", 
        "browser/ui/BUILD.gn",
        "test/BUILD.gn",
    ]
    
    for file_path in critical_files:
        source = Path(file_path)
        if source.exists():
            dest = backup_dir / file_path
            dest.parent.mkdir(parents=True, exist_ok=True)
            try:
                shutil.copy2(str(source), str(dest))
                print(f"  ✓ Backed up {file_path}")
            except (OSError, shutil.Error) as e:
                print(f"  ⚠️  Failed to backup {file_path}: {e}")
    
    print(f"📁 Backup created in {backup_dir}")
    return True

def main():
    parser = argparse.ArgumentParser(description='Run complete Luxxle cleanup process')
    parser.add_argument('--dry-run', action='store_true', 
                       help='Run all scripts in dry-run mode to see what would change')
    parser.add_argument('--skip-backup', action='store_true',
                       help='Skip creating backup files')
    parser.add_argument('--skip-git-check', action='store_true',
                       help='Skip git status check')
    parser.add_argument('--build-only', action='store_true',
                       help='Only fix BUILD.gn files')
    parser.add_argument('--includes-only', action='store_true', 
                       help='Only fix include statements')
    parser.add_argument('--resources-only', action='store_true',
                       help='Only fix resource references')
    
    args = parser.parse_args()
    
    print("🚀 Luxxle Cleanup Script")
    print("=" * 50)
    
    # Safety checks
    if not args.skip_git_check and not check_git_status():
        print("❌ Aborting due to git status check")
        return 1
    
    if not args.skip_backup and not args.dry_run:
        if not create_backup():
            print("❌ Backup creation failed")
            return 1
    
    # Determine which scripts to run
    scripts_to_run = []
    
    if args.build_only:
        scripts_to_run = [('fix_build_dependencies.py', 'BUILD.gn dependencies')]
    elif args.includes_only:
        scripts_to_run = [('fix_includes.py', 'Include statements')]
    elif args.resources_only:
        scripts_to_run = [('fix_resources.py', 'Resource references')]
    else:
        # Full cleanup
        scripts_to_run = [
            ('fix_build_dependencies.py', 'BUILD.gn dependencies'),
            ('fix_includes.py', 'Include statements'),
            ('fix_resources.py', 'Resource references'),
        ]
    
    # Prepare arguments
    script_args = ['--dry-run'] if args.dry_run else []
    
    print(f"\n📋 Will run {len(scripts_to_run)} cleanup scripts:")
    for script, description in scripts_to_run:
        print(f"   • {script} - {description}")
    
    if args.dry_run:
        print("\n🔍 DRY RUN MODE - No files will be modified")
    else:
        print("\n⚠️  LIVE MODE - Files will be modified")
        response = input("Continue? (y/N): ")
        if response.lower() not in ['y', 'yes']:
            print("❌ Aborted by user")
            return 1
    
    # Run the scripts
    success_count = 0
    
    for script, description in scripts_to_run:
        print(f"\n{'='*60}")
        print(f"🔧 {description}")
        print(f"{'='*60}")
        
        if run_script(script, script_args):
            success_count += 1
        else:
            print(f"❌ Failed to run {script}")
            if not args.dry_run:
                response = input("Continue with remaining scripts? (y/N): ")
                if response.lower() not in ['y', 'yes']:
                    break
    
    # Summary
    print(f"\n{'='*60}")
    print(f"📊 SUMMARY")
    print(f"{'='*60}")
    print(f"✅ {success_count}/{len(scripts_to_run)} scripts completed successfully")
    
    if not args.dry_run and success_count > 0:
        print("\n🎯 Next Steps:")
        print("1. Review changes: git diff")
        print("2. Test compilation: ninja -C out/Default chrome")
        print("3. Fix any remaining compilation errors manually")
        print("4. Update search engine to luxxle.com in browser/search_engines/")
        print("5. Commit changes: git add . && git commit -m 'Clean up Brave dependencies'")
    elif args.dry_run:
        print("\n🔍 Dry run completed. Run without --dry-run to apply changes.")
    
    return 0 if success_count == len(scripts_to_run) else 1

if __name__ == '__main__':
    sys.exit(main()) 
#!/usr/bin/env python3
"""
Luxxle Patch Auditor
Categorizes patches to determine which ones to keep vs remove
"""

import os
import re
from pathlib import Path

class PatchAuditor:
    def __init__(self, patches_dir):
        self.patches_dir = Path(patches_dir)
        self.patch_categories = {
            'infrastructure': [],  # Build system, tooling, etc.
            'chromium_enhancement': [],  # UI improvements, bug fixes
            'security': [],  # Security patches (non-Brave specific)
            'brave_features': [],  # Brave-specific features to remove
            'brave_services': [],  # Brave service integration to remove
            'brave_branding': [],  # Brave branding to remove/replace
            'unknown': []  # Need manual review
        }
        
        # Keywords to categorize patches
        self.keywords = {
            'infrastructure': [
                'build', 'config', 'gn', 'gni', 'deps', 'presubmit',
                'android-gradle', 'create_installer', 'branding.gni',
                'BUILD.gn', 'sources.gni', 'BUILDCONFIG'
            ],
            'chromium_enhancement': [
                'ui-views', 'toolbar', 'tabs', 'bookmarks', 'downloads',
                'extensions', 'settings', 'history', 'password_manager',
                'privacy_page', 'sync_controls', 'omnibox', 'location_bar'
            ],
            'security': [
                'safe_browsing', 'content_settings', 'permissions',
                'browsing_data', 'ssl', 'certificate', 'privacy',
                'os_crypt'
            ],
            'brave_features': [
                'brave_vpn', 'brave_wallet', 'brave_shields', 'brave_ads',
                'brave_rewards', 'brave_sync', 'brave_search', 'brave_news',
                'speedreader', 'brave_webtorrent', 'playlist', 'tor',
                'brave_page_graph', 'farbling', 'brave_skus'
            ],
            'brave_services': [
                'brave_stats', 'p3a', 'brave_component_updater',
                'brave_metrics', 'brave_update', 'brave_ping'
            ],
            'brave_branding': [
                'brave_strings', 'brave_resources', 'brave_theme',
                'brave_icons', 'brave_logo'
            ]
        }
    
    def categorize_patch(self, patch_file):
        """Categorize a patch file based on its content and filename"""
        patch_name = patch_file.name.lower()
        
        # Read patch content to understand what it does
        try:
            with open(patch_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read().lower()
        except:
            content = ""
        
        combined_text = f"{patch_name} {content}"
        
        # Check each category
        for category, keywords in self.keywords.items():
            if any(keyword in combined_text for keyword in keywords):
                return category
        
        # Special cases for specific patterns
        if 'chrome-' in patch_name and any(term in patch_name for term in ['ui-', 'browser-', 'renderer-']):
            return 'chromium_enhancement'
        
        if 'components-' in patch_name and not any(brave_term in patch_name for brave_term in ['brave_', 'shields', 'rewards']):
            return 'chromium_enhancement'
        
        return 'unknown'
    
    def audit_all_patches(self):
        """Audit all patches in the patches directory"""
        if not self.patches_dir.exists():
            print(f"❌ Patches directory not found: {self.patches_dir}")
            return
        
        patch_files = list(self.patches_dir.glob("*.patch"))
        print(f"🔍 Found {len(patch_files)} patch files to audit...")
        
        for patch_file in patch_files:
            category = self.categorize_patch(patch_file)
            self.patch_categories[category].append(patch_file.name)
        
        return self.patch_categories
    
    def generate_audit_report(self):
        """Generate a detailed audit report"""
        report_path = self.patches_dir.parent / "patch_audit_report.txt"
        
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write("Luxxle Patch Audit Report\n")
            f.write("=========================\n\n")
            
            total_patches = sum(len(patches) for patches in self.patch_categories.values())
            f.write(f"Total patches audited: {total_patches}\n\n")
            
            # Patches to KEEP
            keep_categories = ['infrastructure', 'chromium_enhancement', 'security']
            keep_count = sum(len(self.patch_categories[cat]) for cat in keep_categories)
            
            f.write("PATCHES TO KEEP (Essential for Luxxle)\n")
            f.write("=" * 40 + "\n")
            f.write(f"Total: {keep_count} patches\n\n")
            
            for category in keep_categories:
                patches = self.patch_categories[category]
                f.write(f"{category.upper()} ({len(patches)} patches):\n")
                for patch in sorted(patches):
                    f.write(f"  KEEP {patch}\n")
                f.write("\n")
            
            # Patches to REMOVE
            remove_categories = ['brave_features', 'brave_services', 'brave_branding']
            remove_count = sum(len(self.patch_categories[cat]) for cat in remove_categories)
            
            f.write("PATCHES TO REMOVE (Brave-specific)\n")
            f.write("=" * 40 + "\n")
            f.write(f"Total: {remove_count} patches\n\n")
            
            for category in remove_categories:
                patches = self.patch_categories[category]
                f.write(f"{category.upper()} ({len(patches)} patches):\n")
                for patch in sorted(patches):
                    f.write(f"  REMOVE {patch}\n")
                f.write("\n")
            
            # Patches needing MANUAL REVIEW
            unknown_patches = self.patch_categories['unknown']
            f.write("PATCHES NEEDING MANUAL REVIEW\n")
            f.write("=" * 40 + "\n")
            f.write(f"Total: {len(unknown_patches)} patches\n\n")
            
            for patch in sorted(unknown_patches):
                f.write(f"  REVIEW {patch}\n")
        
        print(f"📊 Patch audit report saved to: {report_path}")
        return report_path
    
    def generate_removal_script(self):
        """Generate a script to remove Brave-specific patches"""
        script_path = self.patches_dir.parent / "remove_brave_patches.py"
        
        remove_categories = ['brave_features', 'brave_services', 'brave_branding']
        patches_to_remove = []
        for category in remove_categories:
            patches_to_remove.extend(self.patch_categories[category])
        
        with open(script_path, 'w', encoding='utf-8') as f:
            f.write("#!/usr/bin/env python3\n")
            f.write('"""\nRemove Brave-specific patches\n"""\n\n')
            f.write("import os\nfrom pathlib import Path\n\n")
            f.write("patches_to_remove = [\n")
            
            for patch in sorted(patches_to_remove):
                f.write(f'    "{patch}",\n')
            
            f.write("]\n\n")
            f.write("patches_dir = Path('patches')\n")
            f.write("removed_count = 0\n\n")
            f.write("for patch_name in patches_to_remove:\n")
            f.write("    patch_path = patches_dir / patch_name\n")
            f.write("    if patch_path.exists():\n")
            f.write("        print(f'Removing {patch_name}')\n")
            f.write("        patch_path.unlink()\n")
            f.write("        removed_count += 1\n")
            f.write("    else:\n")
            f.write("        print(f'Warning: Not found: {patch_name}')\n\n")
            f.write("print(f'\\nRemoved {removed_count} Brave-specific patches')\n")
        
        # Make script executable
        os.chmod(script_path, 0o755)
        print(f"🧹 Patch removal script created: {script_path}")
        return script_path
    
    def print_summary(self):
        """Print a summary of the audit"""
        total = sum(len(patches) for patches in self.patch_categories.values())
        
        print(f"\n📊 Patch Audit Summary:")
        print(f"Total patches: {total}")
        print(f"")
        
        keep_categories = ['infrastructure', 'chromium_enhancement', 'security']
        remove_categories = ['brave_features', 'brave_services', 'brave_branding']
        
        keep_count = sum(len(self.patch_categories[cat]) for cat in keep_categories)
        remove_count = sum(len(self.patch_categories[cat]) for cat in remove_categories)
        unknown_count = len(self.patch_categories['unknown'])
        
        print(f"✅ To keep: {keep_count} patches")
        print(f"❌ To remove: {remove_count} patches")
        print(f"❓ Manual review: {unknown_count} patches")
        print(f"")
        
        for category, patches in self.patch_categories.items():
            if patches:
                status = "✅" if category in keep_categories else "❌" if category in remove_categories else "❓"
                print(f"{status} {category}: {len(patches)} patches")

def main():
    auditor = PatchAuditor("patches")
    
    print("🔍 Starting patch audit...")
    auditor.audit_all_patches()
    
    auditor.print_summary()
    auditor.generate_audit_report()
    auditor.generate_removal_script()
    
    print("\n✅ Patch audit completed!")

if __name__ == "__main__":
    main() 
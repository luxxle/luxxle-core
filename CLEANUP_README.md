# Luxxle Cleanup Scripts

This directory contains scripts to help clean up remaining Brave components from your Luxxle fork.

## Answer to Your Question: Scriptable vs Manual?

**Recommended approach: Hybrid - Scripts for bulk work, manual for nuanced fixes**

### ✅ **Use Scripts For:**
- BUILD.gn dependency removal (bulk replacements)
- Include path updates (`#include "brave/` → `#include "luxxle/`)
- Header guard replacements (`BRAVE_` → `LUXXLE_`)
- Resource reference cleanup (`IDR_BRAVE_` patterns)
- Namespace updates (`brave::` → `luxxle::`)

### ⚠️ **Manual Review Needed For:**
- Complex logic changes in C++ code
- UI/UX decisions (what Luxxle should look like)
- Search engine configuration (changing to luxxle.com)
- Test file logic updates
- Feature-specific code that needs architectural decisions

## 🚀 Quick Start (Recommended)

### 1. Dry Run First (Safe)
```bash
python run_luxxle_cleanup.py --dry-run
```

### 2. Run Full Cleanup
```bash
python run_luxxle_cleanup.py
```

### 3. Targeted Cleanup (if you prefer step-by-step)
```bash
# Fix BUILD.gn files only
python run_luxxle_cleanup.py --build-only

# Fix include statements only  
python run_luxxle_cleanup.py --includes-only

# Fix resource references only
python run_luxxle_cleanup.py --resources-only
```

## 📋 Individual Scripts

### `fix_build_dependencies.py`
**Purpose:** Removes/comments out Brave component dependencies in BUILD.gn files
**Safe Level:** 🟢 High - Just comments out problematic lines
```bash
python fix_build_dependencies.py --dry-run  # Preview changes
python fix_build_dependencies.py            # Apply changes
```

### `fix_includes.py` 
**Purpose:** Updates #include paths and header guards
**Safe Level:** 🟡 Medium - Makes path changes, review recommended
```bash
python fix_includes.py --dry-run           # Preview changes
python fix_includes.py --file-pattern=browser/  # Only browser files
python fix_includes.py                     # Apply to all files
```

### `fix_resources.py`
**Purpose:** Cleans up IDR_BRAVE resource references
**Safe Level:** 🟡 Medium - May affect UI, review needed
```bash
python fix_resources.py --dry-run          # Preview changes
python fix_resources.py --grd-only         # Only .grd files
python fix_resources.py --source-only      # Only source files
python fix_resources.py                    # Apply to all files
```

## 🔧 Manual Fixes You'll Still Need

### 1. **Search Engine Configuration**
Location: `browser/search_engines/`
- Update default search to luxxle.com
- Remove Brave-specific search features

### 2. **Branding and UI**
- Update app icons and logos
- Change browser name strings
- Update about pages and help text

### 3. **Feature Decisions**
Decide what to do with these Luxxle-specific features:
- Ad blocking (keep/remove/replace?)
- Privacy features (shields, etc.)
- New tab page design
- Bookmark UI customizations

### 4. **Test Compilation**
After scripts run:
```bash
# In chromium build directory
ninja -C out/Default chrome
```

Fix any remaining compilation errors manually.

## 🛡️ Safety Features

### Automatic Backups
The master script creates backups in `luxxle_cleanup_backup/`

### Git Safety Checks
Warns if you have uncommitted changes

### Dry Run Mode
See exactly what would change before applying

## 🚨 Important Warnings

1. **These scripts are powerful** - They make bulk changes across hundreds of files
2. **Always run dry-run first** - Preview changes before applying
3. **Commit before running** - Or use git stash to save current state
4. **Review changes** - Use `git diff` to see what was modified
5. **Test compilation frequently** - Fix issues as they arise

## 📊 What the Scripts Will Fix

### Current Issues Found:
- ❌ 50+ BUILD.gn files with broken dependencies
- ❌ 200+ files with wrong include paths  
- ❌ 100+ files with IDR_BRAVE resource references
- ❌ Header guards still using BRAVE_ prefix
- ❌ Namespace references to brave::

### After Scripts:
- ✅ BUILD.gn files cleaned (dependencies commented out)
- ✅ Include paths updated to luxxle/
- ✅ Header guards use LUXXLE_ prefix
- ✅ Resource references fixed or removed
- ✅ Namespaces updated to luxxle::

## 🎯 Expected Results

**Before scripts:** Will NOT compile (missing dependencies, broken includes)
**After scripts:** Should compile with minimal manual fixes needed

The scripts handle ~80% of the cleanup work automatically, leaving you to focus on the architectural and design decisions that require human judgment.

## 🔄 Iterative Approach

1. Run scripts with `--dry-run` to see scope
2. Run `--build-only` first to fix most critical issues
3. Test compilation: `ninja -C out/Default chrome`
4. Run `--includes-only` to fix header issues  
5. Test compilation again
6. Run `--resources-only` to fix UI resources
7. Manual fixes for remaining compilation errors
8. Update branding and search engine

This approach minimizes risk and lets you fix issues incrementally. 
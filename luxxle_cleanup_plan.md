# Luxxle Cleanup Plan - Brave Components Removal

## Executive Summary
This plan outlines the systematic removal of all remaining Brave components from the Luxxle codebase to create a clean, compilable Chromium fork that can be extended with Luxxle-specific features.

## Phase 1: Critical Build System Cleanup (Priority 1)

### 1.1 Remove Brave-Specific Directories
**Target**: Complete removal of Brave feature directories that will cause build failures
- [ ] `browser/brave_ads/` - Brave's advertising system
- [ ] `browser/brave_adaptive_captcha/` - Brave's captcha system  
- [ ] `browser/ai_chat/` - Brave's AI chat feature
- [ ] `browser/autoplay/` - Brave's autoplay blocking
- [ ] `browser/banners/` - Brave's banner management
- [ ] `ios/brave-ios/` - Complete Brave iOS implementation
- [ ] Any other `brave_*` directories in browser/

### 1.2 Fix DEPS Files
**Target**: Remove all references to `brave/` paths that no longer exist
- [ ] `utility/DEPS` - Remove `+brave/` entries
- [ ] `v8/include/DEPS` - Remove brave buildflags reference
- [ ] `browser/DEPS` - Clean up brave references
- [ ] Scan all DEPS files for remaining `brave/` references

### 1.3 Update BUILD.gn Files
**Target**: Remove brave-specific build targets and dependencies
- [ ] `utility/BUILD.gn` - Remove brave utility dependencies
- [ ] `utility/sources.gni` - Remove brave utility sources
- [ ] `browser/BUILD.gn` - Remove brave browser targets
- [ ] Root `BUILD.gn` - Remove brave-specific configurations

## Phase 2: Source Code Cleanup (Priority 2)

### 2.1 Rename Brave-Named Files
**Target**: Rename files with "brave" in the name to "luxxle" equivalents
- [ ] `browser/brave_browser_main_parts_mac.h` → `browser/luxxle_browser_main_parts_mac.h`
- [ ] `browser/brave_browser_main_parts.h` → `browser/luxxle_browser_main_parts.h`
- [ ] `browser/brave_browser_features.h` → `browser/luxxle_browser_features.h`
- [ ] `browser/brave_app_controller_mac.h` → `browser/luxxle_app_controller_mac.h`
- [ ] Update corresponding `.cc` and `.mm` files
- [ ] Update all `#include` statements referencing renamed files

### 2.2 Update Class Names and Namespaces
**Target**: Rename Brave classes to Luxxle equivalents
- [ ] `BraveBrowserMainParts` → `LuxxleBrowserMainParts`
- [ ] `BraveBrowserFeatures` → `LuxxleBrowserFeatures`
- [ ] `BraveAppController` → `LuxxleAppController`
- [ ] Update all references to renamed classes

## Phase 3: UI and Frontend Cleanup (Priority 3)

### 3.1 Remove Brave UI Dependencies
**Target**: Replace Brave-specific UI components with Chromium equivalents
- [ ] `ui/webui/resources/leo/web_components.ts` - Remove `@brave/leo` imports
- [ ] `ui/webui/resources/br_elements/` - Remove or replace brave elements
- [ ] Update `package.json` to remove `@brave/leo` dependencies
- [ ] Replace brave UI components with standard Chromium components

### 3.2 Update CSS and Styling
**Target**: Remove Brave-specific CSS variables and styling
- [ ] Replace `--brave-toolbar-gradient` with standard variables
- [ ] Update loading shim CSS files in various pages
- [ ] Remove Brave-specific styling from `br_shared_vars.css`
- [ ] Update icon references from `brave-icons` to `luxxle-icons`

### 3.3 Update Resource References
**Target**: Fix resource paths and references
- [ ] Update `page_specific_v3.grdp` brave resource references
- [ ] Fix icon set references in polymer overriding
- [ ] Update manifest.json files removing brave references

## Phase 4: Configuration and Metadata (Priority 4)

### 4.1 Update Configuration Files
**Target**: Remove Brave-specific configurations
- [ ] Remove `.brave_gclient` file
- [ ] Update `package.json` - remove Brave dependencies, update metadata
- [ ] Update `tsconfig*.json` files if they reference Brave paths
- [ ] Update `CONTRIBUTING.md` and other docs

### 4.2 Update Branding and Domains
**Target**: Replace Brave branding with Luxxle
- [ ] Update `luxxle_domains/` service domains configuration
- [ ] Update search engine defaults to point to luxxle.com
- [ ] Update user agent strings and browser identification
- [ ] Update application names and descriptions

## Phase 5: Search Engine Integration (Priority 5)

### 5.1 Configure Luxxle Search Engine
**Target**: Set luxxle.com as the default search engine
- [ ] Update search engine configurations in `browser/search_engines/`
- [ ] Configure default search provider settings
- [ ] Update omnibox search behavior
- [ ] Test search functionality

## Phase 6: Testing and Validation (Priority 6)

### 6.1 Build System Validation
**Target**: Ensure the codebase compiles successfully
- [ ] Run build and fix compilation errors
- [ ] Verify all dependencies are resolved
- [ ] Test on multiple platforms if applicable

### 6.2 Runtime Testing
**Target**: Verify basic browser functionality
- [ ] Test browser startup
- [ ] Test basic navigation
- [ ] Test search functionality
- [ ] Test settings pages

## Implementation Strategy

### Automated Cleanup Scripts
Create Python scripts to automate repetitive tasks:
1. **brave_file_renamer.py** - Rename files and update references
2. **deps_cleaner.py** - Clean DEPS files of brave references  
3. **build_gn_cleaner.py** - Remove brave targets from BUILD.gn files
4. **string_replacer.py** - Mass string replacement for class names

### Manual Tasks
Some tasks require manual intervention:
- Complex BUILD.gn modifications
- UI component replacement decisions
- Testing and validation

## Risk Assessment

### High Risk
- Breaking build system during DEPS/BUILD.gn cleanup
- Missing critical dependencies
- UI functionality breaking

### Medium Risk  
- Search engine integration issues
- Resource loading problems
- Platform-specific build issues

### Mitigation
- Create backups before major changes
- Implement changes incrementally
- Test builds frequently
- Keep detailed change log

## Success Criteria
1. ✅ Codebase compiles without errors
2. ✅ Browser starts and basic functionality works
3. ✅ No references to "brave" in codebase (except in comments/docs)
4. ✅ Luxxle.com set as default search engine
5. ✅ Ready for Luxxle-specific feature development

## Timeline Estimate
- **Phase 1-2**: 2-3 days (Critical path)
- **Phase 3-4**: 2-3 days (UI and config)
- **Phase 5-6**: 1-2 days (Search and testing)
- **Total**: ~1 week of focused development

---
*Created: $(date)*
*Status: Planning Phase* 
# Luxxle Phase 2 Cleanup Plan
## Removing Remaining Brave Components

### Files to Remove from browser/

**Core Brave Files:**
- `brave_content_browser_client.cc/h` → Replace with standard Chromium ContentBrowserClient
- `brave_browser_process_impl.cc/h` → Use standard ChromeBrowserProcess
- `brave_profile_prefs.cc/h` → Merge essential prefs into standard Chrome prefs
- `brave_local_state_prefs.cc/h` → Use standard Chrome local state
- `brave_tab_helpers.cc/h` → Use standard Chrome tab helpers
- `brave_shell_integration.*` → Use standard Chrome shell integration
- `brave_features_internal_names.h` → Remove Brave-specific features
- `brave_drm_tab_helper.*` → Use standard DRM handling

**Test Files to Remove:**
- `brave_*_browsertest.cc`
- `brave_*_unittest.cc`
- `brave_resources_*`

### Directories to Clean/Remove:

**Remove Entirely:**
- `browser/farbling/` → Privacy feature, not needed for basic browser
- `browser/skus/` → Brave payment system
- `browser/psst/` → Brave-specific feature

**Clean/Minimize:**
- `browser/ui/` → Remove Brave-specific UI, keep patches for Chromium UI
- `browser/extensions/` → Keep essential extension support, remove Brave extensions
- `browser/net/` → Keep essential networking, remove Brave-specific networking

### Third-party Modifications:

**Remove from third_party/blink/:**
- `renderer/modules/brave/` → Brave-specific web APIs
- `renderer/core/farbling/` → Fingerprinting protection (Brave-specific)
- `renderer/core/brave_page_graph/` → Brave analytics
- `renderer/platform/brave_audio_farbling_helper.*` → Brave privacy feature

### Build System Cleanup:

**Files to modify:**
- `BUILD.gn` → Remove Brave targets, focus on Chrome builds
- Various `sources.gni` files → Remove Brave source lists
- Remove `brave_*` prefixed build targets

### App/Branding:

**Keep minimal branding:**
- Replace Brave icons with Luxxle branding
- Update string resources to remove Brave references
- Keep essential app structure

### Patches Strategy:

**Categories to keep:**
1. **Infrastructure patches** → Build system, dependency management
2. **Chromium enhancement patches** → UI improvements, bug fixes
3. **Security patches** → General security improvements (not Brave-specific)

**Categories to remove:**
1. **Brave feature patches** → VPN, wallet, shields integration
2. **Brave service patches** → Analytics, updates, sync with Brave services
3. **Brave branding patches** → Brave-specific UI elements

### Implementation Order:

1. **Phase 1: Remove Brave Features**
   - Remove remaining Brave service directories
   - Clean browser/ directory of Brave-specific files
   - Remove Brave-specific third-party modifications

2. **Phase 2: Replace Core Components**
   - Replace BraveContentBrowserClient with ChromeContentBrowserClient base
   - Replace Brave browser process with Chrome browser process
   - Merge essential preferences into Chrome defaults

3. **Phase 3: Clean Build System**
   - Remove Brave build targets
   - Update dependencies
   - Test compilation

4. **Phase 4: Patch Audit**
   - Review all 517 patches
   - Remove Brave-specific functionality patches
   - Keep infrastructure and enhancement patches

### Verification Steps:

1. **Compile test** → Ensure it builds as basic Chromium
2. **Runtime test** → Verify basic browser functionality
3. **Patch test** → Ensure patch system still works for future updates
4. **Size test** → Verify significant reduction in codebase size

This approach will give you a clean Chromium base with the ability to add patches, similar to how Brave extends Chromium, but without any Brave-specific functionality. 
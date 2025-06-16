# Luxxle Brave Component Cleanup Instructions

## Overview
This guide will help you systematically remove Brave components from your Luxxle fork to create a minimal Chromium-based browser while preserving the patch system.

## Prerequisites
1. **Backup your current state**:
   ```bash
   git checkout -b backup-before-cleanup
   git push origin backup-before-cleanup
   ```

2. **Ensure you have Python 3 installed** for running the cleanup script.

## Step 1: Run the Automated Cleanup Script

1. **Execute the cleanup script**:
   ```bash
   python cleanup_script.py
   ```
   
2. **Review the cleanup summary** in `cleanup_summary.txt`

## Step 2: Manual Component Removal

### Remove Remaining Brave Components
Delete these directories if they still exist:

```bash
# Core Brave components
rm -rf components/brave_vpn
rm -rf components/brave_wallet  
rm -rf components/brave_webtorrent
rm -rf components/brave_wayback_machine
rm -rf components/playlist
rm -rf components/speedreader
rm -rf components/tor
rm -rf components/web_discovery
rm -rf components/ai_rewriter
rm -rf components/ntp_background_images
rm -rf components/request_otr

# Browser features
rm -rf browser/brave_vpn
rm -rf browser/brave_wallet
rm -rf browser/brave_stats
rm -rf browser/brave_shields
rm -rf browser/brave_search
rm -rf browser/speedreader
rm -rf browser/playlist
rm -rf browser/tor
rm -rf browser/p3a
rm -rf browser/new_tab
rm -rf browser/ntp_background
rm -rf browser/decentralized_dns
rm -rf browser/cosmetic_filters

# Services
rm -rf services/bat_ads
rm -rf services/bat_rewards
```

## Step 3: Clean Build System

### Remove Brave Build Files
```bash
rm -f brave_paks.gni
rm -f brave_repack_locales.gni
```

### Update Common Build Dependencies
Edit `common/BUILD.gn` and remove references to:
- Brave component dependencies
- Brave-specific includes
- Brave feature flags

## Step 4: Update Application Branding

### Clean App Directory
Remove Brave string files:
```bash
rm -f app/brave_strings.grd
rm -f app/brave_generated_resources.grd
rm -f app/brave_settings_strings.grdp
rm -f app/settings_brave_strings.grdp
rm -f app/brave_command_ids.h
```

### Update String References
1. Edit `app/generated_resources_override.grd`
2. Replace any remaining "Brave" references with "Luxxle"
3. Keep only essential overrides

## Step 5: Clean Dependencies

### Update DEPS File
Edit `DEPS` file and remove:
- `bat-native-*` dependencies
- `brave-intl` repositories
- `playlist-component` 
- Other Brave-specific dependencies

Keep only:
- Essential build tools
- Chromium compatibility patches
- Core Luxxle branding

### Example DEPS cleanup:
```python
# Remove these lines:
"vendor/bat-native-tweetnacl": "...",
"third_party/playlist_component/src": "...",
"components/brave_wallet/browser/zcash/rust/librustzcash/src": "...",
```

## Step 6: Patch System Maintenance

### Preserve Essential Patches
Keep patches that:
- Modify core Chromium behavior
- Fix build issues
- Add necessary performance optimizations
- Maintain compatibility with patch system

### Remove Brave Feature Patches
Remove patches that:
- Add Brave-specific UI elements
- Enable Brave services
- Modify behavior for Brave features

### Test Patch Application
```bash
# Test that remaining patches still apply
python script/apply_patches.py --check
```

## Step 7: Update Build Configuration

The main configuration has been updated in `build/config.gni` with all Brave features disabled.

### Verify Configuration
- All `enable_brave_*` flags are set to `false`
- Product name is set to "Luxxle"
- No enterprise features enabled

## Step 8: Test Build Process

### Initial Build Test
```bash
# Set up build directory
gn gen out/Release --args="is_debug=false is_official_build=true"

# Test compilation
ninja -C out/Release chrome
```

### Build Verification
1. Verify no Brave components are compiled
2. Check for missing dependencies
3. Ensure patches apply correctly
4. Test basic browser functionality

## Step 9: Final Cleanup

### Remove Build Artifacts
```bash
rm -rf win_build_output/
rm -rf node_modules/
```

### Clean Git History (Optional)
If you want to remove Brave components from git history:
```bash
# This is destructive - make sure you have backups
git filter-branch --tree-filter 'rm -rf components/brave_vpn' HEAD
```

## Step 10: Validation

### Functionality Tests
1. **Basic browsing**: Open web pages, navigate
2. **Settings**: Verify settings page loads without Brave options
3. **Extensions**: Test extension compatibility
4. **Performance**: Check resource usage compared to full Brave

### Build Tests
1. **Clean build**: `ninja -C out/Release chrome`
2. **Incremental build**: Make small changes and rebuild
3. **Patch application**: Verify patches still work
4. **Cross-platform**: Test on different platforms if needed

## Maintenance Going Forward

### Regular Tasks
1. **Sync with Chromium**: Regularly update base Chromium version
2. **Patch maintenance**: Update patches for new Chromium versions
3. **Dependency cleanup**: Remove any accidentally added Brave dependencies
4. **Documentation**: Keep track of any custom modifications

### Adding New Features
When adding features to Luxxle:
1. Avoid reintroducing Brave components
2. Use Chromium's extension APIs when possible
3. Keep modifications minimal and well-documented
4. Test compatibility with patch system

## Troubleshooting

### Build Failures
- Check for missing dependencies in cleaned DEPS
- Verify all Brave references are removed from BUILD.gn files
- Ensure patches apply cleanly

### Runtime Issues
- Test with `--disable-features` flag to isolate problems
- Check for remaining Brave service dependencies
- Verify resource files are properly cleaned

### Patch Conflicts
- Update patches to work with cleaned codebase
- Remove patches that depend on removed Brave components
- Test patch application after each major cleanup

## Success Criteria

Your cleanup is complete when:
1. ✅ Browser builds without Brave components
2. ✅ No Brave features appear in UI
3. ✅ Patch system still functions
4. ✅ Resource usage is minimal
5. ✅ All tests pass
6. ✅ Browser functions as expected Chromium fork

## Support

For issues during cleanup:
1. Check `cleanup_summary.txt` for what was removed
2. Verify git status for unexpected changes
3. Test incremental reversions if needed
4. Document any custom modifications needed 
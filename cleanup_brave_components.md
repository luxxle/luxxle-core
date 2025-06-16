# Luxxle Brave Component Removal Guide

This document outlines the systematic removal of Brave-specific components from your Luxxle fork to create a minimal Chromium-based browser.

## Phase 1: Core Component Removal

### 1.1 Components Directory Cleanup
Remove these Brave-specific directories:
- `components/brave_vpn/` - VPN functionality
- `components/brave_wallet/` - Crypto wallet
- `components/brave_webtorrent/` - Torrent support
- `components/brave_wayback_machine/` - Wayback machine integration
- `components/playlist/` - Media playlist
- `components/speedreader/` - Reading mode
- `components/tor/` - Tor integration
- `components/web_discovery/` - Web discovery
- `components/ai_rewriter/` - AI features
- `components/ntp_background_images/` - New tab backgrounds
- `components/request_otr/` - Off-the-record requests

### 1.2 Browser Directory Cleanup
Remove these browser-specific Brave features:
- `browser/brave_vpn/`
- `browser/brave_wallet/`
- `browser/brave_stats/`
- `browser/brave_shields/`
- `browser/brave_search/`
- `browser/profiles/` (Brave-specific profile management)
- `browser/themes/` (Brave themes)
- `browser/sync/` (Brave sync)
- `browser/speedreader/`
- `browser/playlist/`
- `browser/tor/`
- `browser/p3a/` (Privacy analytics)
- `browser/new_tab/` (Brave new tab page)
- `browser/ntp_background/`
- `browser/decentralized_dns/`
- `browser/cosmetic_filters/`

### 1.3 App Directory Cleanup
Remove Brave branding and strings:
- `app/brave_strings.grd`
- `app/brave_generated_resources.grd`
- `app/brave_settings_strings.grdp`
- `app/settings_brave_strings.grdp`
- `app/brave_command_ids.h`

## Phase 2: Build System Cleanup

### 2.1 Remove Brave Build Dependencies
Files to clean up:
- `brave_paks.gni`
- `brave_repack_locales.gni`
- `build/` directory (Brave-specific build scripts)

### 2.2 Update Sources Lists
Clean these files of Brave references:
- All `sources.gni` files
- All `BUILD.gn` files in subdirectories

## Phase 3: Network and Service Cleanup

### 3.1 Network Components
- `net/` - Clean DNS and proxy modifications
- `services/` - Remove Brave-specific services

### 3.2 Extension System
- `extensions/` - Remove Brave extension APIs

## Phase 4: UI and Branding

### 4.1 UI Components
- `ui/` - Remove Brave UI customizations
- `resources/` - Replace Brave assets with minimal branding

### 4.2 Renderer Process
- `renderer/` - Clean Brave content script injections

## Phase 5: Patch System Preservation

### 5.1 Keep Essential Patches
Preserve these patch categories:
- Core Chromium modifications for stability
- Build system patches
- Essential security patches
- Performance optimizations

### 5.2 Remove Brave-Specific Patches
Remove patches that add Brave features:
- VPN-related patches
- Wallet-related patches
- Shields-related patches
- New tab page patches

## Implementation Strategy

1. **Backup Current State**: Create a git branch before major changes
2. **Incremental Removal**: Remove components one category at a time
3. **Build Testing**: Test compilation after each major removal
4. **Patch Validation**: Ensure remaining patches still apply correctly
5. **Minimal Branding**: Replace "Brave" references with "Luxxle" where necessary

## Post-Cleanup Validation

1. Verify clean Chromium compilation
2. Test basic browser functionality
3. Confirm patch system still works
4. Validate minimal resource usage
5. Check for remaining Brave dependencies

## Maintenance Strategy

1. Keep patch system for future Chromium updates
2. Maintain minimal deviation from upstream Chromium
3. Document any remaining customizations
4. Regular cleanup of accumulated Brave references 
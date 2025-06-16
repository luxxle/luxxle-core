#!/usr/bin/env python3
"""
Remove Brave-specific patches
"""

import os
from pathlib import Path

patches_to_remove = [
    "base-threading-thread_restrictions.h.patch",
    "chrome-android-java-src-org-chromium-chrome-browser-contextmenu-ChromeContextMenuPopulator.java.patch",
    "chrome-browser-enterprise-webstore-features.cc.patch",
    "chrome-browser-enterprise-webstore-features.h.patch",
    "chrome-browser-global_keyboard_shortcuts_mac.mm.patch",
    "chrome-browser-importer-external_process_importer_client.h.patch",
    "chrome-browser-net-profile_network_context_service.cc.patch",
    "chrome-browser-net-system_network_context_manager.cc.patch",
    "chrome-browser-notifications-notification_display_service_impl.cc.patch",
    "chrome-browser-prefs-chrome_pref_service_factory.cc.patch",
    "chrome-browser-resources_util.cc.patch",
    "chrome-browser-storage_access_api-storage_access_grant_permission_context.cc.patch",
    "chrome-browser-ui-browser_actions.h.patch",
    "chrome-browser-ui-webui-chrome_web_ui_controller_factory.cc.patch",
    "chrome-browser-ui-webui-print_preview-print_preview_ui.cc.patch",
    "chrome-installer-linux-debian-postrm.patch",
    "chrome-installer-mini_installer-chrome.release.patch",
    "chrome-installer-util-shell_util.cc.patch",
    "components-component_updater-component_updater_service.h.patch",
    "components-download-internal-common-download_item_impl.cc.patch",
    "components-security_interstitials-content-resources-known_interception_disclosure.html.patch",
    "components-sync-engine-sync_scheduler_impl.cc.patch",
    "components-sync-engine-syncer_proto_util.cc.patch",
    "components-sync-protocol-device_info_specifics.proto.patch",
    "components-sync-service-sync_service_impl.h.patch",
    "components-sync-service-sync_stopped_reporter.h.patch",
    "components-sync_device_info-device_info.h.patch",
    "components-update_client-pipeline.cc.patch",
    "components-vector_icons-aggregate_vector_icons.py.patch",
    "content-browser-loader-reconnectable_url_loader_factory.cc.patch",
    "content-browser-renderer_host-navigation_request.cc.patch",
    "content-browser-renderer_host-render_frame_host_impl.cc.patch",
    "third_party-blink-renderer-bindings-core-v8-referrer_script_info.cc.patch",
    "third_party-blink-renderer-bindings-core-v8-referrer_script_info.h.patch",
    "third_party-blink-renderer-bindings-core-v8-v8_script_runner.cc.patch",
    "third_party-blink-renderer-core-frame-navigator_device_memory.idl.patch",
    "third_party-blink-renderer-core-html-canvas-canvas_async_blob_creator.cc.patch",
    "third_party-blink-renderer-core-loader-mixed_content_checker.cc.patch",
    "third_party-blink-renderer-core-loader-modulescript-module_script_loader.cc.patch",
    "third_party-blink-renderer-modules-encryptedmedia-navigator_request_media_key_system_access.cc.patch",
    "third_party-blink-renderer-modules-webaudio-analyser_handler.cc.patch",
    "third_party-blink-renderer-platform-fonts-font_fallback_list.cc.patch",
    "third_party-inspector_protocol-pdl.py.patch",
    "tools-json_schema_compiler-cc_generator.py.patch",
    "tools-perf-core-results_dashboard.py.patch",
    "tools-perf-page_sets-system_health-story_tags.py.patch",
    "ui-base-accelerators-accelerator.cc.patch",
    "ui-native_theme-native_theme_win.h.patch",
]

patches_dir = Path('patches')
removed_count = 0

for patch_name in patches_to_remove:
    patch_path = patches_dir / patch_name
    if patch_path.exists():
        print(f'Removing {patch_name}')
        patch_path.unlink()
        removed_count += 1
    else:
        print(f'Warning: Not found: {patch_name}')

print(f'\nRemoved {removed_count} Brave-specific patches')

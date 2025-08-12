# Copyright 2012 The Chromium Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# This file is used to assign starting resource ids for resources and strings
# used by Chromium.  This is done to ensure that resource ids are unique
# across all the grd files.  If you are adding a new grd file, please add
# a new entry to this file.
#
# The entries below are organized into sections. When adding new entries,
# please use the right section. Try to keep entries in alphabetical order.
#
# - chrome/app/
# - chrome/browser/
# - chrome/ WebUI
# - chrome/ miscellaneous
# - chromeos/
# - components/
# - ios/ (overlaps with chrome/)
# - content/
# - everything else
#
# The range of ID values, which is used by pak files, is from 0 to 2^16 - 1.
#
# IMPORTANT: For update instructions, see README.md.
{
  # The first entry in the file, SRCDIR, is special: It is a relative path from
  # this file to the base of your checkout.
  "SRCDIR": "../..",

  # START chrome/app section.
  #
  # chrome/ and ios/chrome/ must start at the same id.
  # App only use one file depending on whether it is iOS or other platform.
  # Chromium strings and Google Chrome strings must start at the same id.
  # We only use one file depending on whether we're building Chromium or
  # Google Chrome.
  "chrome/app/chromium_strings.grd": {
    "messages": [800],
  },
  "chrome/app/google_chrome_strings.grd": {
    "messages": [800],
  },
  "chrome/app/luxxle_strings.grd": {
    "messages": [800],
  },

  # Leave lots of space for generated_resources since it has most of our
  # strings.
  "chrome/app/generated_resources.grd": {
    # Big alignment since strings (previous item) are frequently added.
    "META": {"join": 2, "align": 200},
    "messages": [1000],
  },

  "chrome/app/resources/locale_settings.grd": {
    # Big alignment since strings (previous item) are frequently added.
    "META": {"align": 1000},
    "messages": [2000],
  },

  # These each start with the same resource id because we only use one
  # file for each build (chromiumos, google_chromeos, linux, mac, or win).
  "chrome/app/resources/locale_settings_chromiumos.grd": {
    # Big alignment since strings (previous item) are frequently added.
    "META": {"align": 100},
    "messages": [2100],
  },
  "chrome/app/resources/locale_settings_google_chromeos.grd": {
    "messages": [2100],
  },
  "chrome/app/resources/locale_settings_linux.grd": {
    "messages": [2100],
  },
  "chrome/app/resources/locale_settings_mac.grd": {
    "messages": [2100],
  },
  "chrome/app/resources/locale_settings_win.grd": {
    "messages": [2100],
  },

  "chrome/app/theme/chrome_unscaled_resources.grd": {
    "META": {"join": 5},
    "includes": [2120],
  },
  "chrome/app/theme/google_chrome/chromeos/chromeos_chrome_internal_strings.grd": {
    "messages": [2140],
  },

  # Leave space for theme_resources since it has many structures.
  "chrome/app/theme/theme_resources.grd": {
    "structures": [2160],
  },
  # END chrome/app section.

  # START chrome/browser section.
  "chrome/browser/browser_resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "includes": [2200],
    "structures": [2220],
  },
  "chrome/browser/dev_ui_browser_resources.grd": {
    "includes": [2240],
  },
  "chrome/browser/nearby_sharing/internal/nearby_share_internal_icons.grd": {
    "includes": [2250],
  },
  "chrome/browser/nearby_sharing/internal/nearby_share_internal_strings.grd": {
    "messages": [2260],
  },
  "chrome/browser/platform_experience/win/resources/platform_experience_win_resources.grd": {
    "includes": [2270],
    "messages": [2280],
  },
  "chrome/browser/recent_tabs/internal/android/java/strings/android_restore_tabs_strings.grd": {
    "messages": [2290],
  },
  "chrome/browser/resources/app_icon/app_icon_resources.grd": {
    "structures": [2320],
  },
  "chrome/browser/resources/chromeos/app_icon/app_icon_resources.grd": {
    "structures": [2330],
  },
  "chrome/browser/resources/chromeos/mako/resources.grd": {
    "META": {"sizes": {"includes": [150]}},
    "includes": [2340],
  },
  "chrome/browser/resources/chromeos/seal/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [2520],
  },
  "chrome/browser/resources/component_extension_resources.grd": {
    "includes": [2580],
    "structures": [2630],
  },
  "chrome/browser/resources/office_web_app/resources.grd": {
    "includes": [2640],
  },
  "chrome/browser/resources/preinstalled_web_apps/resources.grd": {
    "includes": [2650],
  },
  "chrome/browser/test_dummy/internal/android/resources/resources.grd": {
    "includes": [2670],
  },
  # chrome/browser/glic/resources/internal/browser_resources.grd and
  # chrome/browser/glic/resources/browser_resources.grd must share the same id
  # because they define the same resources, but only one of them is built
  # depending on whether src_internal is available.
  "chrome/browser/glic/resources/internal/browser_resources.grd": {
    "messages": [2680],
    "includes": [2690],
  },
  "chrome/browser/glic/resources/browser_resources.grd": {
    "messages": [2680],
    "includes": [2770],
  },
  # END chrome/browser section.

  # START chrome/ WebUI resources section
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/about_sys/resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100, "sizes": {"includes": [10]}},
    "includes": [2850],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/access_code_cast/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [2870],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/accessibility/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [2930],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/app_home/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [2950],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/app_service_internals/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [2980],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/app_settings/resources.grd": {
    "META": {"sizes": {"includes": [45]}},
    "includes": [2990],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/ash/extended_updates/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [3050],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/ash/inline_login/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [3080],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/ash/print_preview/resources.grd": {
    "META": {"sizes": {"includes": [500]}},
    "includes": [3110],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/ash/settings/resources.grd": {
    "META": {"sizes": {"includes": [1000],}},
    "includes": [3690],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/bluetooth_internals/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [4840],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/bookmarks/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [4900],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/browser_switch/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [4960],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/browsing_topics/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [4980],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/certificate_viewer/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5000],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/add_supervision/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5020],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/app_install/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [5040],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/assistant_optin/assistant_optin_resources.grd": {
    "META": {"sizes": {"includes": [80]}},
    "includes": [5050],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/audio/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [5150],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/bluetooth_pairing_dialog/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5190],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/borealis_installer/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [5210],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/borealis_motd/resources.grd": {
    "META": {"sizes": {"includes": [3],}},
    "includes": [5240],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/cloud_upload/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [5250],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/crostini_installer/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [5310],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/crostini_upgrader/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [5330],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/desk_api/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5350],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/edu_coexistence/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [5370],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/emoji_picker/resources.grd": {
    "META": {"sizes": {"includes": [60]}},
    "includes": [5400],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/enterprise_reporting/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [5470],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/gaia_action_buttons/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5500],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/healthd_internals/resources.grd": {
    "META": {"sizes": {"includes": [70]}},
    "includes": [5520],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/internet_config_dialog/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5610],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/internet_detail_dialog/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [5630],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/kerberos/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [5650],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/launcher_internals/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [5660],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/lock_screen_reauth/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [5720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/login/resources.grd": {
    "META": {"sizes": {"includes": [320],}},
    "includes": [5760],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/manage_mirrorsync/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [6130],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/multidevice_internals/resources.grd": {
    "META": {"sizes": {"includes": [35]}},
    "includes": [6150],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/multidevice_setup/multidevice_setup_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [6200],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/nearby_internals/resources.grd": {
    "META": {"sizes": {"includes": [40]}},
    "includes": [6220],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/nearby_share/resources.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [6270],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/network_ui/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [6390],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/notification_tester/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [6420],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/parent_access/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [6430],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/password_change/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [6490],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/remote_maintenance_curtain/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [6530],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/sensor_info/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [6550],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/set_time_dialog/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [6610],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/skyvault/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [6620],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/supervision/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [6640],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/vm/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [6660],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/commerce/product_specifications/resources.grd": {
    "META": {"sizes": {"includes": [70]}},
    "includes": [6670],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/components/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [6760],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/compose/resources.grd": {
    "META": {"sizes": {"includes": [15]}},
    "includes": [6770],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/connectors_internals/resources.grd": {
    "META": {"sizes": {"includes": [15]}},
    "includes": [6790],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/data_sharing/resources.grd": {
   "META": {"sizes": {"includes": [20]}},
    "includes": [6810],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/device_log/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [6840],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/discards/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [6850],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/chromeos/dlp_internals/resources.grd": {
    "META": {"sizes": {"includes": [15]}},
    "includes": [6880],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/downloads/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [6900],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/engagement/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [6960],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/extensions/resources.grd": {
    "META": {"sizes": {"includes": [120],}},
    "includes": [6970],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/family_link_user_internals/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [7110],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/feed/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [7120],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/feed_internals/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [7150],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/feedback/resources.grd": {
    "META": {"sizes": {"includes": [30],}},
    "includes": [7170],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/gaia_auth_host/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [7210],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/glic/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [7240],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/glic/fre/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [7280],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/history/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [7300],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/inline_login/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [7360],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/inspect/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [7380],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/invalidations/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [7390],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/intro/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [7410],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/key_value_pair_viewer_shared/resources.grd": {
   "META": {"sizes": {"includes": [10]}},
    "includes": [7440],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/lens/overlay/resources.grd": {
    "META": {"sizes": {"includes": [90]}},
    "includes": [7460],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/lens/shared/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [7570],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/location_internals/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [7600],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/management/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [7620],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/media/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [7640],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/media_router/cast_feedback/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [7670],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/media_router/internals/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [7690],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/memory_internals/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [7710],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/net_internals/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [7720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/new_tab_footer/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [7750],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/new_tab_page/resources.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [7770],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/new_tab_page/untrusted/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [8000],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/new_tab_page_instant/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [8030],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/new_tab_page_third_party/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [8050],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/omnibox/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [8070],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/omnibox_popup/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [8110],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/on_device_internals/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [8170],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/on_device_translation_internals/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [8200],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/password_manager/resources.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [8210],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/pdf/resources.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [8440],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/predictors/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [8670],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/print_preview/resources.grd": {
    "META": {"sizes": {"includes": [500],}},
    "includes": [8680],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/privacy_sandbox/internals/resources.grd": {
   "META": {"sizes": {"includes": [80],}},
    "includes": [9260],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/privacy_sandbox/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [9360],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/profile_internals/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [9420],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/quota_internals/quota_internals_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [9440],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/sandbox_internals/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [9470],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/saved_tab_groups_unsupported/resources.grd": {
    "META": {"sizes": {"includes": [5]}},
    "includes": [9480],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/search_engine_choice/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [9490],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/segmentation_internals/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [9520],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/settings/resources.grd": {
    "META": {"sizes": {"includes": [500],}},
    "includes": [9540],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/settings_shared/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [10120],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/bookmarks/code_cache_resources.grd": {
    "META": {"sizes": {"includes": [45],}},
    "includes": [10180],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/bookmarks/resources.grd": {
    "META": {"sizes": {"includes": [45],}},
    "includes": [10240],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/commerce/resources.grd": {
    "META": {"sizes": {"includes": [20],}},
    "includes": [10300],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/customize_chrome/code_cache_resources.grd": {
    "META": {"sizes": {"includes": [80],}},
    "includes": [10330],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/customize_chrome/resources.grd": {
    "META": {"sizes": {"includes": [80],}},
    "includes": [10430],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/history/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [10530],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/history_clusters/resources.grd": {
    "META": {"sizes": {"includes": [5],}},
    "includes": [10540],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/read_anything/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [10550],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/reading_list/code_cache_resources.grd": {
    "META": {"sizes": {"includes": [15],}},
    "includes": [10610],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/reading_list/resources.grd": {
    "META": {"sizes": {"includes": [15],}},
    "includes": [10630],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/shared/code_cache_resources.grd": {
    "META": {"sizes": {"includes": [15],}},
    "includes": [10650],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/side_panel/shared/resources.grd": {
    "META": {"sizes": {"includes": [18],}},
    "includes": [10670],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/signin/batch_upload/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [10700],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/signin/history_sync_optin/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [10720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/signin/profile_picker/resources.grd": {
    "META": {"sizes": {"includes": [50],}},
    "includes": [10740],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/signin/resources.grd": {
    "META": {"sizes": {"includes": [90],}},
    "includes": [10800],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/signin/signout_confirmation/resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [10910],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/suggest_internals/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [10930],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/support_tool/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [10950],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/tab_search/resources.grd": {
    "META": {"sizes": {"includes": [90]}},
    "includes": [10990],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/tab_strip/resources.grd": {
    "META": {"sizes": {"includes": [30]}},
    "includes": [11100],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/tts_engine/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [11140],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/usb_internals/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [11170],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/user_education_internals/resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [11200],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/web_app_internals/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [11230],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/webapks/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [11250],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/webui_gallery/resources.grd": {
    "META": {"sizes": {"includes": [90]}},
    "includes": [11270],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/webui_js_error/resources.grd": {
   "META": {"sizes": {"includes": [10],}},
   "includes": [11380],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/browser/resources/whats_new/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [11400],
  },
  # END chrome/ WebUI resources section

  # START chrome/ miscellaneous section.
  "chrome/common/common_resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "includes": [11450],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/common/chromeos/extensions/chromeos_system_extensions_resources.grd": {
    "META": {"sizes": {"includes": [10],}},
    "includes": [11460],
  },
  "chrome/credential_provider/gaiacp/gaia_resources.grd": {
    "includes": [11480],
    "messages": [11490],
  },
  "chrome/renderer/resources/renderer_resources.grd": {
    "includes": [11540],
    "structures": [11610],
  },
  "<(SHARED_INTERMEDIATE_DIR)/chrome/test/data/webui/resources.grd": {
    "META": {"sizes": {"includes": [2500],}},
    "includes": [11620],
  },
  # END chrome/ miscellaneous section.

  # START chromeos/ section.
  "chromeos/chromeos_strings.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "messages": [14550],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/ambient/resources/lottie_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [14600],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/ambient/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [14660],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/camera_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [1000]}},
    "includes": [14720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/color_internals/resources/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [15770],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/common/resources/resources.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [15790],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/connectivity_diagnostics/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [15910],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/demo_mode_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [15970],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/diagnostics_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [16030],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/eche_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [16150],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/face_ml_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [16210],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/file_manager/resources/resources.grd": {
    "META": {"sizes": {"includes": [1000]}},
    "includes": [16270],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/firmware_update_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17320],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/focus_mode/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17380],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/growth_internals/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17440],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/help_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [17500],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/mall/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17620],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/media_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [17680],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/multidevice_debug/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17900],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/os_feedback_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [17960],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/personalization_app/resources/resources.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [18020],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/print_management/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18240],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/recorder_app_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [18300],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/sanitize_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18420],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/scanning/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18480],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/shimless_rma/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18540],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/shortcut_customization_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18600],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/status_area_internals/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18660],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/system_extensions_internals_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/ash/webui/vc_background_ui/resources/resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [18780],
  },
  # END chromeos/ section.

  # START components/ section.
  "components/components_resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "includes": [18900],
  },
  "components/components_strings.grd": {
    "messages": [18950],
  },
  "components/components_chromium_strings.grd": {
    "messages": [19000],
  },
  "components/components_google_chrome_strings.grd": {
    "messages": [19000],
  },
  "components/components_luxxle_strings.grd": {
    "messages": [19000],
  },
  "components/embedder_support/android/java/strings/web_contents_delegate_android_strings.grd": {
    "messages": [19050],
  },
  "components/policy/resources/policy_templates.grd": {
    "structures": [19100],
    "messages": [19150],
  },
  "components/resources/dev_ui_components_resources.grd": {
    "includes": [19200],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/about_ui/about_ui_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [19220],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/autofill_assistant/autofill_assistant_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [19250],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/compose/compose_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [19310],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/constrained_window/constrained_window_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [19340],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/history_clusters_internals/history_clusters_internals_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [19360],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/lens/lens_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [19380],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/optimization_guide_internals/optimization_guide_internals_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [19440],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/password_manager/password_manager_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [19470],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/prefs_internals/prefs_internals_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [19530],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/privacy_sandbox_internals/privacy_sandbox_internals_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [19550],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/safe_browsing/safe_browsing_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [19570],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/security_interstitials/security_interstitials_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [19630],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/signin/signin_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [19690],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/ukm/ukm_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [19720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/vector_icons/vector_icons_resources.grd": {
    "META": {"sizes": {"includes": [1000]}},
    "includes": [19740],
  },
  "<(SHARED_INTERMEDIATE_DIR)/components/resources/webapps/webapps_resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [20790],
  },
  # END components/ section.

  # START ios/ section (overlaps with chrome/ section)
  "ios/chrome/app/resources/ios_resources.grd": {
    "includes": [400],
    "structures": [450],
  },
  "ios/chrome/app/strings/ios_chromium_strings.grd": {
    "messages": [500],
  },
  "ios/chrome/app/strings/ios_google_chrome_strings.grd": {
    "messages": [500],
  },
  "ios/chrome/app/strings/ios_strings.grd": {
    # Big alignment since strings (previous item) are frequently added.
    "META": {"align": 200},
    "messages": [600],
  },
  "ios/chrome/app/theme/ios_theme_resources.grd": {
    # Big alignment since strings (previous item) are frequently added.
    "META": {"align": 200},
    "structures": [800],
  },
  "ios/chrome/share_extension/strings/ios_share_extension_strings.grd": {
    "messages": [900],
  },
  "ios/chrome/search_widget_extension/strings/ios_search_widget_extension_strings.grd": {
    "messages": [950],
  },
  "ios/chrome/search_widget_extension/strings/ios_search_widget_extension_chromium_strings.grd": {
    "messages": [1000],
  },
  "ios/chrome/search_widget_extension/strings/ios_search_widget_extension_google_chrome_strings.grd": {
    "messages": [1000],
  },
  "ios/chrome/content_widget_extension/strings/ios_content_widget_extension_chromium_strings.grd": {
    "messages": [1050],
  },
  "ios/chrome/content_widget_extension/strings/ios_content_widget_extension_google_chrome_strings.grd": {
    "messages": [1050],
  },
  "ios/chrome/widget_kit_extension/strings/ios_widget_kit_extension_strings.grd": {
    "messages": [1100],
  },
  "ios/chrome/credential_provider_extension/strings/ios_credential_provider_extension_strings.grd": {
    "messages": [1150],
  },
  # END ios/ section.

  # START content/ section.
  "content/content_resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "includes": [21000],
  },
  "<(SHARED_INTERMEDIATE_DIR)/content/browser/resources/media/media_internals_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [21020],
  },
  "<(SHARED_INTERMEDIATE_DIR)/content/browser/webrtc/resources/webrtc_internals_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [21050],
  },
  "<(SHARED_INTERMEDIATE_DIR)/content/dev_ui_content_resources.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [21080],
  },
  # END content/ section.

  # START everything else section.
  "android_webview/ui/aw_resources.grd": {
    # Big alignment at start of section.
    "META": {"align": 100},
    "includes": [21200],
  },
  "android_webview/ui/aw_strings.grd": {
    "messages": [21250],
  },

  "ash/ash_strings.grd": {
    "messages": [21300],
  },

  "ash/webui/resources/ash_common_resources.grd": {
    "includes": [21350],
  },

  "device/bluetooth/bluetooth_strings.grd": {
    "messages": [21400],
  },

  "device/fido/fido_strings.grd": {
    "messages": [21450],
  },

  "extensions/browser/resources/extensions_browser_resources.grd": {
    "structures": [21500],
  },
  "extensions/extensions_resources.grd": {
    "includes": [21550],
  },
  "extensions/renderer/resources/extensions_renderer_resources.grd": {
    "includes": [21600],
    "structures": [21650],
  },
  "extensions/strings/extensions_strings.grd": {
    "messages": [21700],
  },

  "mojo/public/js/mojo_bindings_resources.grd": {
    "includes": [21750],
  },

  "net/base/net_resources.grd": {
    "includes": [21800],
  },

  "remoting/resources/remoting_strings.grd": {
    "messages": [21850],
  },

  "services/services_strings.grd": {
    "messages": [21900],
  },

  "skia/skia_resources.grd": {
    "includes": [21950],
  },

  "third_party/blink/public/blink_image_resources.grd": {
    "structures": [22000],
  },
  "third_party/blink/public/blink_resources.grd": {
    "includes": [22050],
  },
  "third_party/blink/renderer/modules/media_controls/resources/media_controls_resources.grd": {
    "includes": [22100],
    "structures": [22150],
  },
  "third_party/blink/public/strings/blink_accessibility_strings.grd": {
    "messages": [22200],
  },
  "third_party/blink/public/strings/blink_strings.grd": {
    "messages": [22250],
  },

  "ui/base/ui_base_resources.grd": {
    "structures": [22300],
  },
  "ui/chromeos/ui_chromeos_resources.grd": {
    "structures": [22350],
  },
  "ui/chromeos/ui_chromeos_strings.grd": {
    "messages": [22400],
  },
  "ui/resources/ui_resources.grd": {
    "structures": [22450],
  },
  "ui/strings/app_locale_settings.grd": {
    "messages": [22500],
  },
  "ui/strings/ax_strings.grd": {
    "messages": [22550],
  },
  "ui/strings/ui_strings.grd": {
    "messages": [22600],
  },
  "ui/views/examples/views_examples_resources.grd": {
    "includes": [22650],
  },
  "ui/views/resources/views_resources.grd": {
    "structures": [22700],
  },
  "ui/webui/resources/webui_resources.grd": {
    "includes": [22750],
    "structures": [22950],
  },

  "weblayer/weblayer_resources.grd": {
    "includes": [23000],
  },
  # END everything else section.
}
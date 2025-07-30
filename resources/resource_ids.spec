# Since recent changes upstream, Chromium is now using IDs up to ~59570 (see end
# of //out/Component/gen/tools/gritsettings/default_resource_ids). We previously
# added our strings after all of the upstream strings, but we are now too close
# to the maximum id (65536) to continue doing that. Instead, we now overlap with
# a section of upstream strings related to ChromeOS/Ash which are unused by us.
# This range runs from 29300-36930, so it should provide a decent amount of
# room for future growth.
{
  "SRCDIR": "../..",
  "luxxle/common/extensions/api/luxxle_api_resources.grd": {
    "includes": [29580],
  },
  "luxxle/components/resources/luxxle_components_resources.grd": {
    "includes": [29600],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_adblock/luxxle_adblock.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [29670],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_new_tab/luxxle_new_tab.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [29680],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_welcome/luxxle_welcome.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [29730],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/browser/resources/settings/luxxle_settings_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [29750],
  },
  "luxxle/app/luxxle_generated_resources.grd": {
    "includes": [29770],
    "messages": [29780],
  },
  "luxxle/app/theme/luxxle_theme_resources.grd": {
    "structures": [30200],
  },
  "luxxle/app/theme/luxxle_unscaled_resources.grd": {
    "includes": [30300],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_webtorrent/luxxle_webtorrent.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [30310],
  },
  "luxxle/components/luxxle_webtorrent/resources.grd": {
    "includes": [30320],
  },
  "luxxle/components/luxxle_rewards/resources/luxxle_rewards_static_resources.grd": {
    "includes": [30350],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-rewards_internals/rewards_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [30380],
  },
  "luxxle/components/resources/luxxle_components_strings.grd": {
    "messages": [30410],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_page/luxxle_wallet_page.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [33510],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_panel/luxxle_wallet_panel.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [33720],
  },
  "luxxle/components/luxxle_extension/extension/resources.grd": {
    "includes": [33920],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_extension/luxxle_extension.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [33940],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-webcompat_reporter/webcompat_reporter.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [33950],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-cosmetic_filters/cosmetic_filters.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [33960],
  },
  "luxxle/components/tor/resources/tor_static_resources.grd": {
    "includes": [33970],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-tor_internals/tor_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [33980],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_script/luxxle_wallet_script.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [33990],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_vpn_panel/luxxle_vpn_panel.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34000],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_shields_panel/luxxle_shields_panel.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34010],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-trezor_bridge/trezor_bridge.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34020],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-market_display/market_display.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [34030],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_private_new_tab/luxxle_private_new_tab.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34080],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-playlist/playlist.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34090],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-ledger_bridge/ledger_bridge.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34100],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-nft_display/nft_display.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34110],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-cookie_list_opt_in/cookie_list_opt_in.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34120],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_speedreader_toolbar/luxxle_speedreader_toolbar.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34130],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_adblock_internals/luxxle_adblock_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34140],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_swap_page/luxxle_wallet_swap_page.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [34250],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_send_page/luxxle_wallet_send_page.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [34300],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_deposit_page/luxxle_wallet_deposit_page.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [34400],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_wallet_fund_wallet_page/luxxle_wallet_fund_wallet_page.grd": {
    "META": {"sizes": {"includes": [80]}},
    "includes": [34500],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-tip_panel/tip_panel.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [34580]
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-ai_chat_ui/ai_chat_ui.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34600],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-skus_internals/skus_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34610],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_news_internals/luxxle_news_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34620],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-line_chart_display/line_chart_display.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34630]
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-ai_rewriter_ui/ai_rewriter_ui.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34640],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-rewards_page/rewards_page.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [34650],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-creator_detection/creator_detection.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34700],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-ads_internals/ads_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34710],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-custom_site_distiller_scripts/custom_site_distiller_scripts.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [34720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/browser/resources/luxxle_education/resources.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34770],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-new_tab_takeover/new_tab_takeover.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [34780],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-luxxle_new_tab_page_refresh/luxxle_new_tab_page_refresh.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [34790],
  },
  "<(SHARED_INTERMEDIATE_DIR)/luxxle/web-ui-email_aliases/email_aliases.grd": {
    "META": {"sizes": {"includes": [40]}},
    "includes": [34810],
  },
  # WARNING: The upstream ChromeOS/Ash strings currently run through 36930. We
  # must be careful not to exceed that maximum when adding new strings here.
}

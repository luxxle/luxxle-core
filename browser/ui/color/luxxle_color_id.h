/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_COLOR_LUXXLE_COLOR_ID_H_
#define LUXXLE_BROWSER_UI_COLOR_LUXXLE_COLOR_ID_H_

// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/luxxle_wayback_machine/buildflags/buildflags.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/browser/ui/color/chrome_color_id.h"

// clang-format off

#define LUXXLE_COMMON_COLOR_IDS                       \
    E_CPONLY(kColorForTest)                          \
    E_CPONLY(kColorIconBase)                         \
    E_CPONLY(kColorMenuItemSubText)                  \
    E_CPONLY(kColorBookmarkBarInstructionsText)      \
    E_CPONLY(kColorBookmarkBarInstructionsLink)      \
    E_CPONLY(kColorLocationBarFocusRing)             \
    E_CPONLY(kColorLocationBarHoveredShadow)         \
    E_CPONLY(kColorDialogDontAskAgainButton)         \
    E_CPONLY(kColorDialogDontAskAgainButtonHovered)  \
    E_CPONLY(kColorTabGroupBackgroundAlpha)          \
    E_CPONLY(kColorWebDiscoveryInfoBarBackground)    \
    E_CPONLY(kColorWebDiscoveryInfoBarMessage)       \
    E_CPONLY(kColorWebDiscoveryInfoBarLink)          \
    E_CPONLY(kColorWebDiscoveryInfoBarNoThanks)      \
    E_CPONLY(kColorWebDiscoveryInfoBarClose)         \
    E_CPONLY(kColorLuxxleDownloadToolbarButtonActive) \
    E_CPONLY(kColorToolbarButtonActivated)

#define LUXXLE_SEARCH_CONVERSION_COLOR_IDS                             \
    E_CPONLY(kColorSearchConversionCloseButton)                       \
    E_CPONLY(kColorSearchConversionBannerTypeBackground)              \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundBorder)        \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundBorderHovered) \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundGradientFrom)  \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundGradientTo)    \
    E_CPONLY(kColorSearchConversionBannerTypeDescText)                \
    E_CPONLY(kColorSearchConversionButtonBorder)                      \
    E_CPONLY(kColorSearchConversionButtonBackground)                  \
    E_CPONLY(kColorSearchConversionButtonBackgroundHovered)           \
    E_CPONLY(kColorSearchConversionButtonText)                        \
    E_CPONLY(kColorSearchConversionButtonCaratRight)                  \
    E_CPONLY(kColorSearchConversionButtonCloseButton)                 \
    E_CPONLY(kColorSearchConversionButtonCloseButtonHovered)          \
    E_CPONLY(kColorSearchConversionButtonShadow1)                     \
    E_CPONLY(kColorSearchConversionButtonShadow2)

#define LUXXLE_SIDEBAR_COLOR_IDS                               \
    E_CPONLY(kColorSidebarAddBubbleBackground)                \
    E_CPONLY(kColorSidebarAddBubbleHeaderText)                \
    E_CPONLY(kColorSidebarAddBubbleItemTextBackgroundHovered) \
    E_CPONLY(kColorSidebarAddBubbleItemTextHovered)           \
    E_CPONLY(kColorSidebarAddBubbleItemTextNormal)            \
    E_CPONLY(kColorSidebarAddButtonDisabled)                  \
    E_CPONLY(kColorSidebarArrowBackgroundHovered)             \
    E_CPONLY(kColorSidebarArrowDisabled)                      \
    E_CPONLY(kColorSidebarArrowNormal)                        \
    E_CPONLY(kColorSidebarButtonBase)                         \
    E_CPONLY(kColorSidebarButtonPressed)                      \
    E_CPONLY(kColorSidebarItemDragIndicator)                  \
    E_CPONLY(kColorSidebarSeparator)                          \
    E_CPONLY(kColorSidebarPanelHeaderSeparator)               \
    E_CPONLY(kColorSidebarPanelHeaderBackground)              \
    E_CPONLY(kColorSidebarPanelHeaderTitle)                   \
    E_CPONLY(kColorSidebarPanelHeaderButton)                  \
    E_CPONLY(kColorSidebarPanelHeaderButtonHovered)

#if BUILDFLAG(ENABLE_SPEEDREADER)
#define LUXXLE_SPEEDREADER_COLOR_IDS      \
  E_CPONLY(kColorSpeedreaderIcon)        \
  E_CPONLY(kColorSpeedreaderToggleThumb) \
  E_CPONLY(kColorSpeedreaderToggleTrack) \
  E_CPONLY(kColorSpeedreaderToolbarBackground) \
  E_CPONLY(kColorSpeedreaderToolbarBorder) \
  E_CPONLY(kColorSpeedreaderToolbarForeground) \
  E_CPONLY(kColorSpeedreaderToolbarButtonHover) \
  E_CPONLY(kColorSpeedreaderToolbarButtonActive) \
  E_CPONLY(kColorSpeedreaderToolbarButtonActiveText)\
  E_CPONLY(kColorSpeedreaderToolbarButtonBorder)
#else
#define LUXXLE_SPEEDREADER_COLOR_IDS
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#define LUXXLE_VPN_COLOR_IDS                        \
   E_CPONLY(kColorLuxxleVpnButtonIconConnected)     \
    E_CPONLY(kColorLuxxleVpnButtonIconDisconnected) \
    E_CPONLY(kColorLuxxleVpnButtonIconError)        \
    E_CPONLY(kColorLuxxleVpnButtonBackgroundNormal) \
    E_CPONLY(kColorLuxxleVpnButtonErrorBackgroundNormal)
#else
#define LUXXLE_VPN_COLOR_IDS
#endif

#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
#define LUXXLE_WAYBACK_MACHINE_COLOR_IDS      \
    E_CPONLY(kColorWaybackMachineURLLoaded)  \
    E_CPONLY(kColorWaybackMachineURLNotAvailable)
#else
#define LUXXLE_WAYBACK_MACHINE_COLOR_IDS
#endif

// Unfortunately, we can't have a defined(TOOLKIT_VIEWS) guard here
// as luxxle_color_mixer depends on this without deps to //ui/views:flags.
// But it's safe have without the guard as this file is included only when
// !is_android.
#define LUXXLE_VERTICAL_TAB_COLOR_IDS                        \
    E_CPONLY(kColorLuxxleVerticalTabSeparator)               \
    E_CPONLY(kColorLuxxleVerticalTabActiveBackground)        \
    E_CPONLY(kColorLuxxleVerticalTabHoveredBackground)       \
    E_CPONLY(kColorLuxxleVerticalTabInactiveBackground)      \
    E_CPONLY(kColorLuxxleVerticalTabNTBIconColor)            \
    E_CPONLY(kColorLuxxleVerticalTabNTBTextColor)            \
    E_CPONLY(kColorLuxxleVerticalTabNTBShortcutTextColor)

#define LUXXLE_SHARED_PINNED_TAB_COLOR_IDS                    \
    E_CPONLY(kColorLuxxleSharedPinnedTabDummyViewBackground)  \
    E_CPONLY(kColorLuxxleSharedPinnedTabDummyViewTitle)       \
    E_CPONLY(kColorLuxxleSharedPinnedTabDummyViewDescription) \
    E_CPONLY(kColorLuxxleSharedPinnedTabDummyViewThumbnailBorder)


#define LUXXLE_SPLIT_VIEW_COLOR_IDS \
    E_CPONLY(kColorLuxxleSplitViewTileBackgroundHorizontal) \
    E_CPONLY(kColorLuxxleSplitViewTileBackgroundVertical)   \
    E_CPONLY(kColorLuxxleSplitViewTileBackgroundBorder)     \
    E_CPONLY(kColorLuxxleSplitViewTileDivider)              \
    E_CPONLY(kColorLuxxleSplitViewActiveWebViewBorder)      \
    E_CPONLY(kColorLuxxleSplitViewInactiveWebViewBorder)    \
    E_CPONLY(kColorLuxxleSplitViewMenuButtonIcon)           \
    E_CPONLY(kColorLuxxleSplitViewMenuButtonBackground)     \
    E_CPONLY(kColorLuxxleSplitViewMenuButtonBorder)         \
    E_CPONLY(kColorLuxxleSplitViewMenuItemIcon)             \
    E_CPONLY(kColorLuxxleSplitViewUrl)


#define LUXXLE_PLAYLIST_COLOR_IDS                                      \
    E_CPONLY(kColorLuxxlePlaylistAddedIcon)                            \
    E_CPONLY(kColorLuxxlePlaylistCheckedIcon)                          \
    E_CPONLY(kColorLuxxlePlaylistSelectedBackground)                   \
    E_CPONLY(kColorLuxxlePlaylistListBorder)                           \
    E_CPONLY(kColorLuxxlePlaylistMoveDialogDescription)                \
    E_CPONLY(kColorLuxxlePlaylistMoveDialogCreatePlaylistAndMoveTitle) \
    E_CPONLY(kColorLuxxlePlaylistNewPlaylistDialogNameLabel)           \
    E_CPONLY(kColorLuxxlePlaylistNewPlaylistDialogItemsLabel)          \
    E_CPONLY(kColorLuxxlePlaylistTextInteractive)

#define LUXXLE_OMNIBOX_COLOR_IDS \
    E_CPONLY(kColorLuxxleOmniboxResultViewSeparator)

#define LUXXLE_EXTENSION_MENU_COLOR_IDS \
    E_CPONLY(kColorLuxxleExtensionMenuIcon)

#define LUXXLE_APP_MENU_COLOR_IDS \
    E_CPONLY(kColorLuxxleAppMenuAccentColor)

#define LUXXLE_COLOR_IDS               \
    LUXXLE_COMMON_COLOR_IDS            \
    LUXXLE_SEARCH_CONVERSION_COLOR_IDS \
    LUXXLE_SIDEBAR_COLOR_IDS           \
    LUXXLE_SPEEDREADER_COLOR_IDS       \
    LUXXLE_VPN_COLOR_IDS               \
    LUXXLE_VERTICAL_TAB_COLOR_IDS      \
    LUXXLE_SHARED_PINNED_TAB_COLOR_IDS \
    LUXXLE_SPLIT_VIEW_COLOR_IDS        \
    LUXXLE_PLAYLIST_COLOR_IDS          \
    LUXXLE_OMNIBOX_COLOR_IDS           \
    LUXXLE_WAYBACK_MACHINE_COLOR_IDS   \
    LUXXLE_EXTENSION_MENU_COLOR_IDS    \
    LUXXLE_APP_MENU_COLOR_IDS

#include "ui/color/color_id_macros.inc"

enum LuxxleColorIds : ui::ColorId {
  kLuxxleColorsStart = kChromeColorsEnd,

  LUXXLE_COLOR_IDS

  kLuxxleColorsEnd,
};

#include "ui/color/color_id_macros.inc"  // NOLINT

// clang-format on

#endif  // LUXXLE_BROWSER_UI_COLOR_LUXXLE_COLOR_ID_H_

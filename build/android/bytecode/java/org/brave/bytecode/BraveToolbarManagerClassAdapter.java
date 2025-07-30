/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleToolbarManagerClassAdapter extends LuxxleClassVisitor {
    static String sToolbarManagerClassName = "org/chromium/chrome/browser/toolbar/ToolbarManager";
    static String sLuxxleToolbarManagerClassName =
            "org/chromium/chrome/browser/toolbar/LuxxleToolbarManager";

    public LuxxleToolbarManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sToolbarManagerClassName, sLuxxleToolbarManagerClassName);

        deleteField(sLuxxleToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mCallbackController");
        makeProtectedField(sToolbarManagerClassName, "mCallbackController");

        deleteField(sLuxxleToolbarManagerClassName, "mBottomControlsStacker");
        makeProtectedField(sToolbarManagerClassName, "mBottomControlsStacker");

        deleteField(sLuxxleToolbarManagerClassName, "mFullscreenManager");
        makeProtectedField(sToolbarManagerClassName, "mFullscreenManager");

        deleteField(sLuxxleToolbarManagerClassName, "mActivityTabProvider");
        makeProtectedField(sToolbarManagerClassName, "mActivityTabProvider");

        deleteField(sLuxxleToolbarManagerClassName, "mAppThemeColorProvider");
        makeProtectedField(sToolbarManagerClassName, "mAppThemeColorProvider");

        deleteField(sLuxxleToolbarManagerClassName, "mScrimManager");
        makeProtectedField(sToolbarManagerClassName, "mScrimManager");

        deleteField(sLuxxleToolbarManagerClassName, "mMenuButtonCoordinator");
        makeProtectedField(sToolbarManagerClassName, "mMenuButtonCoordinator");

        deleteField(sLuxxleToolbarManagerClassName, "mToolbarTabController");
        makeProtectedField(sToolbarManagerClassName, "mToolbarTabController");

        deleteField(sLuxxleToolbarManagerClassName, "mLocationBar");
        makeProtectedField(sToolbarManagerClassName, "mLocationBar");

        deleteField(sLuxxleToolbarManagerClassName, "mActionModeController");
        makeProtectedField(sToolbarManagerClassName, "mActionModeController");

        deleteField(sLuxxleToolbarManagerClassName, "mLocationBarModel");
        makeProtectedField(sToolbarManagerClassName, "mLocationBarModel");

        deleteField(sLuxxleToolbarManagerClassName, "mToolbar");
        makeProtectedField(sToolbarManagerClassName, "mToolbar");

        deleteField(sLuxxleToolbarManagerClassName, "mBookmarkModelSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBookmarkModelSupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mLayoutManager");
        makeProtectedField(sToolbarManagerClassName, "mLayoutManager");

        deleteField(sLuxxleToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");
        makeProtectedField(sToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mTabModelSelector");
        makeProtectedField(sToolbarManagerClassName, "mTabModelSelector");

        deleteField(sLuxxleToolbarManagerClassName, "mIncognitoStateProvider");
        makeProtectedField(sToolbarManagerClassName, "mIncognitoStateProvider");

        deleteField(sLuxxleToolbarManagerClassName, "mBottomSheetController");
        makeProtectedField(sToolbarManagerClassName, "mBottomSheetController");

        deleteField(sLuxxleToolbarManagerClassName, "mTabContentManager");
        makeProtectedField(sToolbarManagerClassName, "mTabContentManager");

        deleteField(sLuxxleToolbarManagerClassName, "mTabCreatorManager");
        makeProtectedField(sToolbarManagerClassName, "mTabCreatorManager");

        deleteField(sLuxxleToolbarManagerClassName, "mModalDialogManagerSupplier");
        makeProtectedField(sToolbarManagerClassName, "mModalDialogManagerSupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mTabObscuringHandler");
        makeProtectedField(sToolbarManagerClassName, "mTabObscuringHandler");

        deleteField(sLuxxleToolbarManagerClassName, "mReadAloudControllerSupplier");
        makeProtectedField(sToolbarManagerClassName, "mReadAloudControllerSupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mTopUiThemeColorProvider");
        makeProtectedField(sToolbarManagerClassName, "mTopUiThemeColorProvider");

        deleteField(sLuxxleToolbarManagerClassName, "mCurrentOrientation");
        makeProtectedField(sToolbarManagerClassName, "mCurrentOrientation");

        deleteField(sLuxxleToolbarManagerClassName, "mInitializedWithNative");
        makeProtectedField(sToolbarManagerClassName, "mInitializedWithNative");

        deleteField(sLuxxleToolbarManagerClassName, "mTabGroupUiOneshotSupplier");
        makeProtectedField(sToolbarManagerClassName, "mTabGroupUiOneshotSupplier");

        deleteField(sLuxxleToolbarManagerClassName, "mUndoBarThrottle");
        makeProtectedField(sToolbarManagerClassName, "mUndoBarThrottle");

        makePublicMethod(sToolbarManagerClassName, "onOrientationChange");
        addMethodAnnotation(
                sLuxxleToolbarManagerClassName, "onOrientationChange", "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateBookmarkButtonStatus");
        addMethodAnnotation(sLuxxleToolbarManagerClassName, "updateBookmarkButtonStatus",
                "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateReloadState");
        deleteMethod(sLuxxleToolbarManagerClassName, "updateReloadState");
    }
}

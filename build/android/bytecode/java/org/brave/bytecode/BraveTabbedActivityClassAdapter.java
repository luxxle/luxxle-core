/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabbedActivityClassAdapter extends LuxxleClassVisitor {
    static String sChromeTabbedActivityClassName =
            "org/chromium/chrome/browser/ChromeTabbedActivity";
    static String sLuxxleActivityClassName = "org/chromium/chrome/browser/app/LuxxleActivity";
    static String sTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedRootUiCoordinator";
    static String sTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedAppMenuPropertiesDelegate";
    static String sLuxxleTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/appmenu/LuxxleTabbedAppMenuPropertiesDelegate";
    static String sChromeTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/ChromeTabCreator";
    static String sLuxxleTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/LuxxleTabCreator";
    static String sAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/AppMenuPropertiesDelegateImpl";
    static String sLuxxleAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/LuxxleAppMenuPropertiesDelegateImpl";
    static String sCustomTabAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/customtabs/CustomTabAppMenuPropertiesDelegate";

    public LuxxleTabbedActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeTabbedActivityClassName, sLuxxleActivityClassName);

        changeSuperName(
                sTabbedAppMenuPropertiesDelegateClassName,
                sLuxxleAppMenuPropertiesDelegateImplClassName);

        changeSuperName(
                sCustomTabAppMenuPropertiesDelegateClassName,
                sLuxxleAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(
                sTabbedAppMenuPropertiesDelegateClassName,
                sLuxxleTabbedAppMenuPropertiesDelegateClassName);

        redirectConstructor(sChromeTabCreatorClassName, sLuxxleTabCreatorClassName);

        makePublicMethod(sChromeTabbedActivityClassName, "hideOverview");

        deleteMethod(sChromeTabbedActivityClassName, "supportsDynamicColors");

        makePublicMethod(sChromeTabbedActivityClassName, "maybeHandleUrlIntent");
        changeMethodOwner(
                sChromeTabbedActivityClassName, "maybeHandleUrlIntent", sLuxxleActivityClassName);
        makeProtectedField(sChromeTabbedActivityClassName, "mLayoutManager");
        makeProtectedField(sChromeTabbedActivityClassName, "mMultiInstanceManager");
    }
}

/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_BOOKMARKS_EXPORTER_LUXXLE_BOOKMARKS_EXPORTER_H_
#define LUXXLE_IOS_BROWSER_API_BOOKMARKS_EXPORTER_LUXXLE_BOOKMARKS_EXPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LuxxleBookmarksExporterState) {
  LuxxleBookmarksExporterStateCompleted,
  LuxxleBookmarksExporterStateStarted,
  LuxxleBookmarksExporterStateCancelled,
  LuxxleBookmarksExporterStateErrorCreatingFile,
  LuxxleBookmarksExporterStateErrorWritingHeader,
  LuxxleBookmarksExporterStateErrorWritingNodes
};

@class IOSBookmarkNode;

OBJC_EXPORT
@interface LuxxleBookmarksExporter : NSObject
- (instancetype)init;

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(LuxxleBookmarksExporterState))listener;

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(LuxxleBookmarksExporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_BOOKMARKS_EXPORTER_LUXXLE_BOOKMARKS_EXPORTER_H_

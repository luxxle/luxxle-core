/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_BOOKMARKS_IMPORTER_LUXXLE_BOOKMARKS_IMPORTER_H_
#define LUXXLE_IOS_BROWSER_API_BOOKMARKS_IMPORTER_LUXXLE_BOOKMARKS_IMPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LuxxleBookmarksImporterState) {
  LuxxleBookmarksImporterStateCompleted,
  LuxxleBookmarksImporterStateAutoCompleted,
  LuxxleBookmarksImporterStateStarted,
  LuxxleBookmarksImporterStateCancelled
};

OBJC_EXPORT
@interface LuxxleImportedBookmark : NSObject
@property(nonatomic, readonly) bool inToolbar;
@property(nonatomic, readonly) bool isFolder;
@property(nullable, nonatomic, readonly, copy) NSURL* url;
@property(nullable, nonatomic, readonly, copy) NSArray<NSString*>* path;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly, copy) NSDate* creationTime;
@end

OBJC_EXPORT
@interface LuxxleBookmarksImporter : NSObject
- (instancetype)init;

- (void)cancel;

- (void)importFromFile:(NSString*)filePath
    topLevelFolderName:(NSString*)folderName
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(LuxxleBookmarksImporterState,
                        NSArray<LuxxleImportedBookmark*>* _Nullable))listener;

- (void)importFromArray:(NSArray<LuxxleImportedBookmark*>*)bookmarks
     topLevelFolderName:(NSString*)folderName
           withListener:(void (^)(LuxxleBookmarksImporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_BOOKMARKS_IMPORTER_LUXXLE_BOOKMARKS_IMPORTER_H_

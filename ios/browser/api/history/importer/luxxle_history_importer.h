/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_HISTORY_IMPORTER_LUXXLE_HISTORY_IMPORTER_H_
#define LUXXLE_IOS_BROWSER_API_HISTORY_IMPORTER_LUXXLE_HISTORY_IMPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LuxxleHistoryImporterState) {
  LuxxleHistoryImporterStateCompleted,
  LuxxleHistoryImporterStateAutoCompleted,
  LuxxleHistoryImporterStateStarted,
  LuxxleHistoryImporterStateCancelled
};

OBJC_EXPORT
@interface LuxxleImportedHistory : NSObject
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) NSInteger visitCount;
@property(nonatomic, readonly) NSDate* lastVisitDate;
@end

OBJC_EXPORT
@interface LuxxleHistoryImporter : NSObject
- (instancetype)init;

- (void)cancel;

- (void)importFromFile:(NSString*)filePath
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(LuxxleHistoryImporterState,
                        NSArray<LuxxleImportedHistory*>* _Nullable))listener;

- (void)importFromArray:(NSArray<LuxxleImportedHistory*>*)historyItems
           withListener:(void (^)(LuxxleHistoryImporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_HISTORY_IMPORTER_LUXXLE_HISTORY_IMPORTER_H_

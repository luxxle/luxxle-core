// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_PASSWORD_IMPORTER_LUXXLE_PASSWORD_IMPORTER_H_
#define LUXXLE_IOS_BROWSER_API_PASSWORD_IMPORTER_LUXXLE_PASSWORD_IMPORTER_H_

#include <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger LuxxlePasswordImportEntryStatus
    NS_TYPED_ENUM NS_SWIFT_NAME(LuxxlePasswordImportEntry.Status);

OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusNone;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusUnknownError;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusMissingPassword;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusMissingURL;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusInvalidURL;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongURL;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongPassword;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongUsername;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusConflictProfile;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusConflictAccount;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongNote;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongConcatenatedNote;
OBJC_EXPORT LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusValid;

OBJC_EXPORT
@interface LuxxlePasswordImportEntry : NSObject
@property(nonatomic, readonly) LuxxlePasswordImportEntryStatus status;
@property(nonatomic, readonly) NSInteger id;
@property(nonatomic, readonly) NSString* username;
@property(nonatomic, readonly) NSString* password;
@end

typedef NSInteger LuxxlePasswordImporterResultsStatus
    NS_TYPED_ENUM NS_SWIFT_NAME(LuxxlePasswordImporterResults.Status);

OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusNone;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusUnknownError;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusSuccess;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusIOError;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusBadFormat;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusDismissed;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusMaxFileSize;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusImportAlreadyActive;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusNumPasswordsExceeded;
OBJC_EXPORT LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusConflicts;

OBJC_EXPORT
NS_SWIFT_NAME(LuxxlePasswordImporter.Results)
@interface LuxxlePasswordImporterResults : NSObject
@property(nonatomic, readonly) LuxxlePasswordImporterResultsStatus status;
@property(nonatomic, readonly) NSString* fileName;
@property(nonatomic, readonly) NSUInteger numberImported;
@property(nonatomic, readonly)
    NSArray<LuxxlePasswordImportEntry*>* displayedEntries;
@end

OBJC_EXPORT
@interface LuxxlePasswordImporter : NSObject
- (void)importPasswords:(NSString*)fileName
             completion:(void (^)(LuxxlePasswordImporterResults*))completion;

- (void)continueImport:(NSArray<LuxxlePasswordImportEntry*>*)entriesToReplace
            completion:(void (^)(LuxxlePasswordImporterResults*))completion;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_PASSWORD_IMPORTER_LUXXLE_PASSWORD_IMPORTER_H_

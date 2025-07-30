/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/bookmarks/importer/luxxle_bookmarks_importer.h"

#include <vector>

#include "base/apple/foundation_util.h"
#include "base/base_paths.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/path_service.h"
#include "base/stl_util.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "luxxle/ios/browser/api/bookmarks/importer/bookmark_html_reader.h"
#include "luxxle/ios/browser/api/bookmarks/importer/bookmarks_importer.h"
#include "luxxle/ios/browser/api/bookmarks/importer/imported_bookmark_entry.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"
#import "net/base/apple/url_conversions.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation LuxxleImportedBookmark
- (instancetype)initFromChromiumImportedBookmark:
    (const ImportedBookmarkEntry&)entry {
  if ((self = [super init])) {
    NSMutableArray<NSString*>* paths = [[NSMutableArray alloc] init];
    for (const auto& path : entry.path) {
      [paths addObject:base::SysUTF16ToNSString(path)];
    }

    _inToolbar = entry.in_toolbar;
    _isFolder = entry.is_folder;
    _url = net::NSURLWithGURL(entry.url);
    _path = paths;
    _title = base::SysUTF16ToNSString(entry.title);
    _creationTime =
        [NSDate dateWithTimeIntervalSince1970:entry.creation_time
                                                  .InSecondsFSinceUnixEpoch()];
  }
  return self;
}

- (ImportedBookmarkEntry)toChromiumImportedBookmark {
  std::vector<std::u16string> paths;
  for (NSString* path in self.path) {
    paths.push_back(base::SysNSStringToUTF16(path));
  }

  ImportedBookmarkEntry entry;
  entry.creation_time = base::Time::FromSecondsSinceUnixEpoch(
      [self.creationTime timeIntervalSince1970]);
  entry.url = net::GURLWithNSURL(self.url);
  entry.title = base::SysNSStringToUTF16(self.title);
  entry.in_toolbar = self.inToolbar;
  entry.path = paths;
  return entry;
}
@end

@interface LuxxleBookmarksImporter () {
  scoped_refptr<base::SequencedTaskRunner> import_thread_;
}
@property(atomic) bool cancelled;  // atomic
@end

@implementation LuxxleBookmarksImporter
- (instancetype)init {
  if ((self = [super init])) {
    self.cancelled = false;

    import_thread_ = base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
  }
  return self;
}

- (void)dealloc {
  [self cancel];
}

- (void)cancel {
  self.cancelled = true;
}

- (void)importFromFile:(NSString*)filePath
    topLevelFolderName:(NSString*)folderName
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(LuxxleBookmarksImporterState,
                        NSArray<LuxxleImportedBookmark*>* _Nullable))listener {
  __weak LuxxleBookmarksImporter* weakSelf = self;

  auto start_import = ^{
    // Import cancelled as the importer has been deallocated
    __strong LuxxleBookmarksImporter* importer = weakSelf;
    if (!importer) {
      listener(LuxxleBookmarksImporterStateStarted, nullptr);
      listener(LuxxleBookmarksImporterStateCancelled, nullptr);
      return;
    }

    // In Chromium, this is IDS_BOOKMARK_GROUP (804)
    std::u16string top_level_folder_name = base::SysNSStringToUTF16(folderName);
    base::FilePath source_file_path = base::apple::NSStringToFilePath(filePath);

    listener(LuxxleBookmarksImporterStateStarted, nullptr);
    std::vector<ImportedBookmarkEntry> bookmarks;
    bookmark_html_reader::ImportBookmarksFile(
        base::BindRepeating(^{
          return [importer isImporterCancelled];
        }),
        base::BindRepeating(^(const GURL& url) {
          return [importer canImportURL:url];
        }),
        source_file_path, &bookmarks, nullptr);

    if (!bookmarks.empty() && ![importer isImporterCancelled]) {
      if (automaticImport) {
        // Import into the Profile/ProfileIOS on the main-thread.
        web::GetUIThreadTaskRunner({})->PostTask(
            FROM_HERE,
            base::BindOnce(
                ^(std::vector<ImportedBookmarkEntry> items,
                  const std::u16string& top_most_folder_name) {
                  BookmarksImporter::AddBookmarks(top_most_folder_name, items);
                  listener(LuxxleBookmarksImporterStateAutoCompleted, nullptr);
                },
                std::move(bookmarks), top_level_folder_name));
      } else {
        listener(LuxxleBookmarksImporterStateCompleted,
                 [importer convertToIOSImportedBookmarks:bookmarks]);
      }
    } else {
      listener(LuxxleBookmarksImporterStateCancelled, nullptr);
    }
  };

  // Run the importer on the sequenced task runner.
  import_thread_->PostTask(FROM_HERE, base::BindOnce(start_import));
}

- (void)importFromArray:(NSArray<LuxxleImportedBookmark*>*)bookmarks
     topLevelFolderName:(NSString*)folderName
           withListener:(void (^)(LuxxleBookmarksImporterState))listener {
  __weak LuxxleBookmarksImporter* weakSelf = self;

  auto start_import = ^{
    // Import cancelled as the importer has been deallocated
    __strong LuxxleBookmarksImporter* importer = weakSelf;
    if (!importer) {
      listener(LuxxleBookmarksImporterStateStarted);
      listener(LuxxleBookmarksImporterStateCancelled);
      return;
    }

    // In Chromium, this is IDS_BOOKMARK_GROUP (804)
    std::u16string top_level_folder_name = base::SysNSStringToUTF16(folderName);

    listener(LuxxleBookmarksImporterStateStarted);
    BookmarksImporter::AddBookmarks(
        top_level_folder_name,
        [importer convertToChromiumImportedBookmarks:bookmarks]);
    listener(LuxxleBookmarksImporterStateCompleted);
  };

  // Import into the Profile/ProfileIOS on the main-thread.
  web::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
                                           base::BindOnce(start_import));
}

// MARK: - Private

- (bool)isImporterCancelled {
  return self.cancelled;
}

// Returns true if |url| has a valid scheme that we allow to import. We
// filter out the URL with a unsupported scheme.
- (bool)canImportURL:(const GURL&)url {
  // The URL is not valid.
  if (!url.is_valid()) {
    return false;
  }

  // Filter out the URLs with unsupported schemes.
  const char* const kInvalidSchemes[] = {"wyciwyg", "place", "about", "chrome"};
  for (size_t i = 0; i < std::size(kInvalidSchemes); ++i) {
    if (UNSAFE_TODO(url.SchemeIs(kInvalidSchemes[i]))) {
      return false;
    }
  }

  return true;
}

// Converts an array of Chromium imported bookmarks to iOS imported bookmarks.
- (NSArray<LuxxleImportedBookmark*>*)convertToIOSImportedBookmarks:
    (const std::vector<ImportedBookmarkEntry>&)bookmarks {
  NSMutableArray<LuxxleImportedBookmark*>* results =
      [[NSMutableArray alloc] init];
  for (const auto& bookmark : bookmarks) {
    LuxxleImportedBookmark* imported_bookmark = [[LuxxleImportedBookmark alloc]
        initFromChromiumImportedBookmark:bookmark];
    [results addObject:imported_bookmark];
  }
  return results;
}

// Converts an array of iOS imported bookmarks to Chromium imported bookmarks.
- (std::vector<ImportedBookmarkEntry>)convertToChromiumImportedBookmarks:
    (NSArray<LuxxleImportedBookmark*>*)bookmarks {
  std::vector<ImportedBookmarkEntry> results;
  for (LuxxleImportedBookmark* bookmark in bookmarks) {
    results.push_back([bookmark toChromiumImportedBookmark]);
  }
  return results;
}
@end

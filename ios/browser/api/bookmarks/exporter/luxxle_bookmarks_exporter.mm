/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/bookmarks/exporter/luxxle_bookmarks_exporter.h"

#include <vector>

#include "base/apple/foundation_util.h"
#include "base/base_paths.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/path_service.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/uuid.h"
#include "base/values.h"
#include "luxxle/ios/browser/api/bookmarks/luxxle_bookmarks_api.h"
#include "luxxle/ios/browser/api/bookmarks/exporter/bookmark_html_writer.h"
#include "luxxle/ios/browser/api/bookmarks/exporter/bookmarks_encoder.h"
#include "components/bookmarks/browser/bookmark_node.h"
#include "components/bookmarks/browser/bookmark_uuids.h"
#include "components/strings/grit/components_strings.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"
#import "net/base/apple/url_conversions.h"
#include "ui/base/l10n/l10n_util.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

class LuxxleBookmarksExportObserver : public BookmarksExportObserver {
 public:
  LuxxleBookmarksExportObserver(
      base::OnceCallback<void(LuxxleBookmarksExporterState)> on_export_finished);
  void OnExportFinished(Result result) override;

 private:
  base::OnceCallback<void(LuxxleBookmarksExporterState)> _on_export_finished;
};

LuxxleBookmarksExportObserver::LuxxleBookmarksExportObserver(
    base::OnceCallback<void(LuxxleBookmarksExporterState)> on_export_finished)
    : _on_export_finished(std::move(on_export_finished)) {}

void LuxxleBookmarksExportObserver::OnExportFinished(Result result) {
  switch (result) {
    case Result::kSuccess:
      std::move(_on_export_finished).Run(LuxxleBookmarksExporterStateCompleted);
      break;
    case Result::kCouldNotCreateFile:
      std::move(_on_export_finished)
          .Run(LuxxleBookmarksExporterStateErrorCreatingFile);
      break;
    case Result::kCouldNotWriteHeader:
      std::move(_on_export_finished)
          .Run(LuxxleBookmarksExporterStateErrorWritingHeader);
      break;
    case Result::kCouldNotWriteNodes:
      std::move(_on_export_finished)
          .Run(LuxxleBookmarksExporterStateErrorWritingNodes);
      break;
    default:
      delete this;
      NOTREACHED();
  }
  delete this;
}

@interface IOSBookmarkNode(BookmarksExporter)
- (void)setNativeParent:(bookmarks::BookmarkNode*)parent;
@end

@interface LuxxleBookmarksExporter () {
  scoped_refptr<base::SequencedTaskRunner> export_thread_;
}
@end

@implementation LuxxleBookmarksExporter

- (instancetype)init {
  if ((self = [super init])) {
    // This work must be done on the UI thread because it currently relies on
    // fetching information from ProfileIOS which is main-thread bound
    export_thread_ = web::GetUIThreadTaskRunner({});
  }
  return self;
}

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(LuxxleBookmarksExporterState))listener {
  __weak LuxxleBookmarksExporter* weakSelf = self;

  auto start_export = ^{
    // Export cancelled as the exporter has been deallocated
    __strong LuxxleBookmarksExporter* exporter = weakSelf;
    if (!exporter) {
      listener(LuxxleBookmarksExporterStateStarted);
      listener(LuxxleBookmarksExporterStateCancelled);
      return;
    }

    DCHECK(GetApplicationContext());

    base::FilePath destination_file_path =
        base::apple::NSStringToFilePath(filePath);

    listener(LuxxleBookmarksExporterStateStarted);

    std::vector<ProfileIOS*> profiles =
        GetApplicationContext()->GetProfileManager()->GetLoadedProfiles();
    ProfileIOS* last_used_profile = profiles.at(0);

    bookmark_html_writer::WriteBookmarks(
        last_used_profile, destination_file_path,
        new LuxxleBookmarksExportObserver(base::BindOnce(listener)));
  };

  export_thread_->PostTask(FROM_HERE, base::BindOnce(start_export));
}

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(LuxxleBookmarksExporterState))listener {
  if ([bookmarks count] == 0) {
    listener(LuxxleBookmarksExporterStateStarted);
    listener(LuxxleBookmarksExporterStateCompleted);
    return;
  }

  __weak LuxxleBookmarksExporter* weakSelf = self;

  auto start_export = ^{
    // Export cancelled as the exporter has been deallocated
    __strong LuxxleBookmarksExporter* exporter = weakSelf;
    if (!exporter) {
      listener(LuxxleBookmarksExporterStateStarted);
      listener(LuxxleBookmarksExporterStateCancelled);
      return;
    }

    listener(LuxxleBookmarksExporterStateStarted);
    base::FilePath destination_file_path =
        base::apple::NSStringToFilePath(filePath);

    // Create artificial nodes
    auto bookmark_bar_node = [exporter getBookmarksBarNode];
    auto other_folder_node = [exporter getOtherBookmarksNode];
    auto mobile_folder_node = [exporter getMobileBookmarksNode];

    for (IOSBookmarkNode* bookmark : bookmarks) {
      // We export as the |mobile_bookmarks_node| by default.
      [bookmark setNativeParent:mobile_folder_node.get()];
    }

    auto encoded_bookmarks = ios::bookmarks_encoder::Encode(
        bookmark_bar_node.get(), other_folder_node.get(),
        mobile_folder_node.get());
    bookmark_html_writer::WriteBookmarks(
        std::move(encoded_bookmarks), destination_file_path,
        new LuxxleBookmarksExportObserver(base::BindOnce(listener)));
  };

  export_thread_->PostTask(FROM_HERE, base::BindOnce(start_export));
}

// MARK: - Internal artificial nodes used for exporting arbitrary bookmarks to a file

- (std::unique_ptr<bookmarks::BookmarkNode>)getRootNode {
  return std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/0, base::Uuid::ParseLowercase(bookmarks::kRootNodeUuid), GURL());
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getBookmarksBarNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/1, base::Uuid::ParseLowercase(bookmarks::kBookmarkBarNodeUuid),
      GURL());
  node->SetTitle(l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_FOLDER_NAME));
  return node;
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getOtherBookmarksNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/2, base::Uuid::ParseLowercase(bookmarks::kOtherBookmarksNodeUuid),
      GURL());
  node->SetTitle(l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_OTHER_FOLDER_NAME));
  return node;
}

- (std::unique_ptr<bookmarks::BookmarkNode>)getMobileBookmarksNode {
  auto node = std::make_unique<bookmarks::BookmarkNode>(
      /*id=*/3, base::Uuid::ParseLowercase(bookmarks::kMobileBookmarksNodeUuid),
      GURL());
  node->SetTitle(
      l10n_util::GetStringUTF16(IDS_BOOKMARK_BAR_MOBILE_FOLDER_NAME));
  return node;
}
@end

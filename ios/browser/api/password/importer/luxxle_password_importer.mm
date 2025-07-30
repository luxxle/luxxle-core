// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/api/password/importer/luxxle_password_importer.h"

#include "base/apple/foundation_util.h"
#include "base/base_paths.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/task_traits.h"
#include "base/task/thread_pool.h"
#include "luxxle/components/password_manager/core/browser/import/safari_password_importer.h"
#include "components/keyed_service/core/service_access_type.h"
#include "components/password_manager/core/browser/password_store/password_store_interface.h"
#include "components/password_manager/core/browser/ui/saved_passwords_presenter.h"
#include "ios/chrome/browser/affiliations/model/ios_chrome_affiliation_service_factory.h"
#include "ios/chrome/browser/passwords/model/ios_chrome_account_password_store_factory.h"
#include "ios/chrome/browser/passwords/model/ios_chrome_profile_password_store_factory.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "ios/chrome/browser/webauthn/model/ios_passkey_model_factory.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"

// MARK: - LuxxlePasswordImportEntryStatus

LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusNone =
    static_cast<NSInteger>(password_manager::SafariImportEntry::Status::NONE);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusUnknownError = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::UNKNOWN_ERROR);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusMissingPassword = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::MISSING_PASSWORD);
LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusMissingURL =
    static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::MISSING_URL);
LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusInvalidURL =
    static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::INVALID_URL);
LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusLongURL =
    static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::LONG_URL);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongPassword = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::LONG_PASSWORD);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongUsername = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::LONG_USERNAME);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusConflictProfile = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::CONFLICT_PROFILE);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusConflictAccount = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::CONFLICT_ACCOUNT);
LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusLongNote =
    static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::LONG_NOTE);
LuxxlePasswordImportEntryStatus const
    LuxxlePasswordImportEntryStatusLongConcatenatedNote = static_cast<NSInteger>(
        password_manager::SafariImportEntry::Status::LONG_CONCATENATED_NOTE);
LuxxlePasswordImportEntryStatus const LuxxlePasswordImportEntryStatusValid =
    static_cast<NSInteger>(password_manager::SafariImportEntry::Status::VALID);

// MARK: - LuxxlePasswordImporterResultsStatus

LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusNone = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::NONE);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusUnknownError = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::UNKNOWN_ERROR);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusSuccess = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::SUCCESS);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusIOError = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::IO_ERROR);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusBadFormat = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::BAD_FORMAT);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusDismissed = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::DISMISSED);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusMaxFileSize = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::MAX_FILE_SIZE);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusImportAlreadyActive = static_cast<
        NSInteger>(
        password_manager::SafariImportResults::Status::IMPORT_ALREADY_ACTIVE);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusNumPasswordsExceeded = static_cast<
        NSInteger>(
        password_manager::SafariImportResults::Status::NUM_PASSWORDS_EXCEEDED);
LuxxlePasswordImporterResultsStatus const
    LuxxlePasswordImporterResultsStatusConflicts = static_cast<NSInteger>(
        password_manager::SafariImportResults::Status::CONFLICTS);

// MARK: - LuxxlePasswordImportEntry

@implementation LuxxlePasswordImportEntry
- (instancetype)initWithEntry:
    (const password_manager::SafariImportEntry&)entry {
  if ((self = [super init])) {
  }
  return self;
}
@end

// MARK: - LuxxlePasswordImporterResults

@interface LuxxlePasswordImporterResults () {
  password_manager::SafariImportResults results_;
}
@end

@implementation LuxxlePasswordImporterResults
- (instancetype)initWithResults:
    (const password_manager::SafariImportResults&)results {
  if ((self = [super init])) {
    results_ = results;
  }
  return self;
}

- (LuxxlePasswordImporterResultsStatus)status {
  return static_cast<LuxxlePasswordImporterResultsStatus>(results_.status);
}

- (NSString*)fileName {
  return base::SysUTF8ToNSString(results_.file_name);
}

- (NSUInteger)numberImported {
  return results_.number_imported;
}

- (NSArray<LuxxlePasswordImportEntry*>*)displayedEntries {
  NSMutableArray* result = [[NSMutableArray alloc] init];

  for (const auto& entry : results_.displayed_entries) {
    [result addObject:[[LuxxlePasswordImportEntry alloc] initWithEntry:entry]];
  }

  return [result copy];
}
@end

// MARK: - LuxxlePasswordImporter

namespace {
// Returns a passkey model instance if the feature is enabled.
webauthn::PasskeyModel* MaybeGetPasskeyModel(ProfileIOS* profile) {
  return IOSPasskeyModelFactory::GetInstance()->GetForProfile(profile);
}

}  // namespace

@interface LuxxlePasswordImporter () {
  std::unique_ptr<password_manager::SavedPasswordsPresenter> _presenter;
  std::unique_ptr<password_manager::SafariPasswordImporter> _safari_importer;
}
@end

@implementation LuxxlePasswordImporter

- (void)importPasswords:(NSString*)filePath
             completion:(void (^)(LuxxlePasswordImporterResults*))completion {
  __weak LuxxlePasswordImporter* weakSelf = self;

  auto start_import = ^{
    __strong LuxxlePasswordImporter* importer = weakSelf;
    if (!importer) {
      return;
    }

    std::vector<ProfileIOS*> profiles =
        GetApplicationContext()->GetProfileManager()->GetLoadedProfiles();
    ProfileIOS* last_used_profile = profiles.at(0);

    importer->_presenter =
        std::make_unique<password_manager::SavedPasswordsPresenter>(
            IOSChromeAffiliationServiceFactory::GetForProfile(
                last_used_profile),
            IOSChromeProfilePasswordStoreFactory::GetForProfile(
                last_used_profile, ServiceAccessType::EXPLICIT_ACCESS),
            IOSChromeAccountPasswordStoreFactory::GetForProfile(
                last_used_profile, ServiceAccessType::EXPLICIT_ACCESS),
            MaybeGetPasskeyModel(last_used_profile));

    importer->_safari_importer =
        std::make_unique<password_manager::SafariPasswordImporter>(
            importer->_presenter.get());

    // Execute the importer

    importer->_presenter->Init(base::BindOnce(^{
      if (!importer) {
        return;
      }

      base::FilePath path = base::apple::NSStringToFilePath(filePath);
      auto password_store =
          password_manager::PasswordForm::Store::kProfileStore;

      importer->_safari_importer->Import(
          path, password_store,
          base::BindOnce(
              ^(const password_manager::SafariImportResults& results) {
                completion([[LuxxlePasswordImporterResults alloc]
                    initWithResults:results]);
              }));
    }));
  };

  web::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
                                           base::BindOnce(start_import));
}

- (void)continueImport:(NSArray<LuxxlePasswordImportEntry*>*)entriesToReplace
            completion:(void (^)(LuxxlePasswordImporterResults*))completion {
  __weak LuxxlePasswordImporter* weakSelf = self;

  auto continue_import = ^{
    __strong LuxxlePasswordImporter* importer = weakSelf;
    if (!importer) {
      return;
    }

    std::vector<int> entry_ids;
    for (LuxxlePasswordImportEntry* entry in entriesToReplace) {
      entry_ids.push_back(entry.id);
    }

    importer->_safari_importer->ContinueImport(
        entry_ids,
        base::BindOnce(^(const password_manager::SafariImportResults& results) {
          completion(
              [[LuxxlePasswordImporterResults alloc] initWithResults:results]);
        }));
  };

  web::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
                                           base::BindOnce(continue_import));
}

@end

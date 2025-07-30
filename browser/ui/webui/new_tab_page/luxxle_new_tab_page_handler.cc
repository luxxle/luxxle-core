// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/new_tab_page/luxxle_new_tab_page_handler.h"

#include <algorithm>
#include <optional>
#include <utility>

#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/browser/ntp_background/custom_background_file_manager.h"
#include "luxxle/browser/ntp_background/ntp_background_prefs.h"
#include "luxxle/browser/ntp_background/view_counter_service_factory.h"
#include "luxxle/browser/ui/webui/new_tab_page/luxxle_new_tab_ui.h"
#include "luxxle/components/luxxle_search_conversion/p3a.h"
#include "luxxle/components/luxxle_search_conversion/pref_names.h"
#include "luxxle/components/luxxle_search_conversion/types.h"
#include "luxxle/components/luxxle_search_conversion/utils.h"
#include "luxxle/components/ntp_background_images/browser/ntp_background_images_data.h"
#include "luxxle/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "luxxle/components/ntp_background_images/browser/url_constants.h"
#include "luxxle/components/ntp_background_images/browser/view_counter_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/themes/theme_syncable_service.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/chrome_select_file_policy.h"
#include "chrome/grit/generated_resources.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/search_engine_type.h"
#include "components/search_engines/template_url.h"
#include "components/tabs/public/tab_interface.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/referrer.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/page_transition_types.h"
#include "ui/base/window_open_disposition.h"
#include "ui/base/window_open_disposition_utils.h"
#include "ui/shell_dialogs/selected_file_info.h"
#include "url/gurl.h"
#include "url/origin.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#include "luxxle/browser/ui/luxxle_vpn/luxxle_vpn_controller.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

namespace {

bool IsNTPPromotionEnabled(Profile* profile) {
  if (!luxxle_search_conversion::IsNTPPromotionEnabled(
          profile->GetPrefs(),
          TemplateURLServiceFactory::GetForProfile(profile))) {
    return false;
  }

  auto* service =
      ntp_background_images::ViewCounterServiceFactory::GetForProfile(profile);
  if (!service) {
    return false;
  }

  // Only show promotion if current wallpaper is not sponsored images.
  std::optional<base::Value::Dict> data =
      service->GetCurrentWallpaperForDisplay();
  if (data) {
    if (const auto is_background =
            data->FindBool(ntp_background_images::kIsBackgroundKey)) {
      return is_background.value();
    }
  }
  return false;
}

}  // namespace

LuxxleNewTabPageHandler::LuxxleNewTabPageHandler(
    mojo::PendingReceiver<luxxle_new_tab_page::mojom::PageHandler>
        pending_page_handler,
    mojo::PendingRemote<luxxle_new_tab_page::mojom::Page> pending_page,
    Profile* profile,
    content::WebContents* web_contents)
    : page_handler_(this, std::move(pending_page_handler)),
      page_(std::move(pending_page)),
      profile_(profile),
      web_contents_(web_contents),
      file_manager_(std::make_unique<CustomBackgroundFileManager>(profile_)) {
  InitForSearchPromotion();
}

LuxxleNewTabPageHandler::~LuxxleNewTabPageHandler() = default;

void LuxxleNewTabPageHandler::InitForSearchPromotion() {
  // If promotion is disabled for this loading, we do nothing.
  // If some condition is changed and it can be enabled, promotion
  // will be shown at the next NTP loading.
  if (!IsNTPPromotionEnabled(profile_)) {
    return;
  }

  // Observing user's dismiss or default search provider change to hide
  // promotion from NTP while NTP is loaded.
  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      luxxle_search_conversion::prefs::kDismissed,
      base::BindRepeating(&LuxxleNewTabPageHandler::OnSearchPromotionDismissed,
                          base::Unretained(this)));
  template_url_service_observation_.Observe(
      TemplateURLServiceFactory::GetForProfile(profile_));

  luxxle_search_conversion::p3a::RecordPromoShown(
      g_browser_process->local_state(),
      luxxle_search_conversion::ConversionType::kNTP);
}

void LuxxleNewTabPageHandler::ChooseLocalCustomBackground() {
  // Early return if the select file dialog is already active.
  if (select_file_dialog_) {
    return;
  }

  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, std::make_unique<ChromeSelectFilePolicy>(web_contents_));
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.allowed_paths = ui::SelectFileDialog::FileTypeInfo::NATIVE_PATH;
  file_types.extensions.resize(1);
  file_types.extensions[0].push_back(FILE_PATH_LITERAL("jpg"));
  file_types.extensions[0].push_back(FILE_PATH_LITERAL("jpeg"));
  file_types.extensions[0].push_back(FILE_PATH_LITERAL("png"));
  file_types.extensions[0].push_back(FILE_PATH_LITERAL("gif"));
  file_types.extension_description_overrides.push_back(
      l10n_util::GetStringUTF16(IDS_UPLOAD_IMAGE_FORMAT));
  select_file_dialog_->SelectFile(
      ui::SelectFileDialog::SELECT_OPEN_MULTI_FILE, std::u16string(),
      profile_->last_selected_directory(), &file_types, 0,
      base::FilePath::StringType(), web_contents_->GetTopLevelNativeWindow(),
      nullptr);
}

void LuxxleNewTabPageHandler::UseCustomImageBackground(
    const std::string& selected_background) {
  auto decoded_background = selected_background;
  if (!decoded_background.empty()) {
    decoded_background =
        CustomBackgroundFileManager::Converter(GURL(decoded_background))
            .To<std::string>();
  }

  auto pref = NTPBackgroundPrefs(profile_->GetPrefs());
  pref.SetType(NTPBackgroundPrefs::Type::kCustomImage);
  pref.SetSelectedValue(decoded_background);
  pref.SetShouldUseRandomValue(decoded_background.empty());

  OnBackgroundUpdated();
}

void LuxxleNewTabPageHandler::GetCustomImageBackgrounds(
    GetCustomImageBackgroundsCallback callback) {
  std::vector<luxxle_new_tab_page::mojom::CustomBackgroundPtr> backgrounds;
  for (const auto& name :
       NTPBackgroundPrefs(profile_->GetPrefs()).GetCustomImageList()) {
    auto value = luxxle_new_tab_page::mojom::CustomBackground::New();
    value->url = CustomBackgroundFileManager::Converter(name).To<GURL>();
    backgrounds.push_back(std::move(value));
  }

  std::move(callback).Run(std::move(backgrounds));
}

void LuxxleNewTabPageHandler::RemoveCustomImageBackground(
    const std::string& background) {
  if (background.empty()) {
    return;
  }

  auto file_path = CustomBackgroundFileManager::Converter(GURL(background),
                                                          file_manager_.get())
                       .To<base::FilePath>();
  file_manager_->RemoveImage(
      file_path,
      base::BindOnce(&LuxxleNewTabPageHandler::OnRemoveCustomImageBackground,
                     weak_factory_.GetWeakPtr(), file_path));
}

void LuxxleNewTabPageHandler::UseLuxxleBackground(
    const std::string& selected_background) {
  // Call ntp custom background images service.
  auto pref = NTPBackgroundPrefs(profile_->GetPrefs());
  pref.SetType(NTPBackgroundPrefs::Type::kLuxxle);
  pref.SetSelectedValue(selected_background);
  pref.SetShouldUseRandomValue(selected_background.empty());

  OnBackgroundUpdated();
}

void LuxxleNewTabPageHandler::TryLuxxleSearchPromotion(const std::string& input,
                                                     bool open_new_tab) {
  const GURL promo_url = luxxle_search_conversion::GetPromoURL(input);
  auto window_open_disposition = WindowOpenDisposition::CURRENT_TAB;
  if (open_new_tab) {
    window_open_disposition = WindowOpenDisposition::NEW_BACKGROUND_TAB;
  }

  web_contents_->OpenURL(
      content::OpenURLParams(
          promo_url, content::Referrer(), window_open_disposition,
          ui::PageTransition::PAGE_TRANSITION_FORM_SUBMIT, false),
      /*navigation_handle_callback=*/{});

  luxxle_search_conversion::p3a::RecordPromoTrigger(
      g_browser_process->local_state(),
      luxxle_search_conversion::ConversionType::kNTP);
}

void LuxxleNewTabPageHandler::DismissLuxxleSearchPromotion() {
  luxxle_search_conversion::SetDismissed(profile_->GetPrefs());
}

void LuxxleNewTabPageHandler::IsSearchPromotionEnabled(
    IsSearchPromotionEnabledCallback callback) {
  std::move(callback).Run(IsNTPPromotionEnabled(profile_));
}

void LuxxleNewTabPageHandler::NotifySearchPromotionDisabledIfNeeded() const {
  // If enabled, we don't do anything. When NTP is reloaded or opened,
  // user will see promotion.
  if (IsNTPPromotionEnabled(profile_)) {
    return;
  }

  // Hide promotion when it's disabled.
  page_->OnSearchPromotionDisabled();
}

void LuxxleNewTabPageHandler::OnSearchPromotionDismissed() {
  NotifySearchPromotionDisabledIfNeeded();
}

void LuxxleNewTabPageHandler::UseColorBackground(const std::string& color,
                                                bool use_random_color) {
  if (use_random_color) {
    DCHECK(color == luxxle_new_tab_page::mojom::kRandomSolidColorValue ||
           color == luxxle_new_tab_page::mojom::kRandomGradientColorValue)
        << "When |use_random_color| is true, |color| should be "
           "kRandomSolidColorValue or kRandomGradientColorValue";
  }

  auto background_pref = NTPBackgroundPrefs(profile_->GetPrefs());
  background_pref.SetType(NTPBackgroundPrefs::Type::kColor);
  background_pref.SetSelectedValue(color);
  background_pref.SetShouldUseRandomValue(use_random_color);

  OnBackgroundUpdated();
}

void LuxxleNewTabPageHandler::GetSearchEngines(
    GetSearchEnginesCallback callback) {
  auto* service = TemplateURLServiceFactory::GetForProfile(profile_);
  CHECK(service);

  auto urls = service->GetTemplateURLs();
  std::vector<luxxle_new_tab_page::mojom::SearchEngineInfoPtr> search_engines;
  for (TemplateURL* template_url : urls) {
    if (template_url->GetBuiltinEngineType() !=
        BuiltinEngineType::KEYWORD_MODE_PREPOPULATED_ENGINE) {
      continue;
    }
    auto search_engine = luxxle_new_tab_page::mojom::SearchEngineInfo::New();
    search_engine->prepopulate_id = template_url->prepopulate_id();
    search_engine->host = GURL(template_url->url()).host();
    search_engine->name = base::UTF16ToUTF8(template_url->short_name());
    search_engine->keyword = base::UTF16ToUTF8(template_url->keyword());
    search_engine->favicon_url = template_url->favicon_url();
    search_engines.push_back(std::move(search_engine));
  }

  std::move(callback).Run(std::move(search_engines));
}

void LuxxleNewTabPageHandler::SearchWhatYouTyped(const std::string& host,
                                                const std::string& query,
                                                bool alt_key,
                                                bool ctrl_key,
                                                bool meta_key,
                                                bool shift_key) {
  auto* service = TemplateURLServiceFactory::GetForProfile(profile_);
  CHECK(service);

  auto* template_url = service->GetTemplateURLForHost(host);
  DCHECK(template_url);
  if (!template_url) {
    return;
  }

  GURL search_url = template_url->GenerateSearchURL(
      service->search_terms_data(), base::UTF8ToUTF16(query));

  const WindowOpenDisposition disposition =
      ui::DispositionFromClick(false, alt_key, ctrl_key, meta_key, shift_key);
  content::OpenURLParams params(search_url, content::Referrer(), disposition,
                                ui::PAGE_TRANSITION_FROM_ADDRESS_BAR, false);
  web_contents_->OpenURL(params, /*navigation_handle_callback=*/{});
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
void LuxxleNewTabPageHandler::RefreshVPNState() {
  auto* vpn_service =
      luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile_);
  vpn_service->ReloadPurchasedState();
}

void LuxxleNewTabPageHandler::LaunchVPNPanel() {
  auto* tab = tabs::TabInterface::GetFromContents(web_contents_);
  CHECK(tab);
  tab->GetBrowserWindowInterface()
      ->GetFeatures()
      .luxxle_vpn_controller()
      ->ShowLuxxleVPNBubble(/* show_select */ true);
}

void LuxxleNewTabPageHandler::OpenVPNAccountPage(
    luxxle_vpn::mojom::ManageURLType type) {
  auto* tab = tabs::TabInterface::GetFromContents(web_contents_);
  CHECK(tab);
  tab->GetBrowserWindowInterface()
      ->GetFeatures()
      .luxxle_vpn_controller()
      ->OpenVPNAccountPage(type);
}

void LuxxleNewTabPageHandler::ReportVPNWidgetUsage() {
  auto* vpn_service =
      luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile_);
  vpn_service->luxxle_vpn_metrics()->RecordWidgetUsage(true);
}
#endif

bool LuxxleNewTabPageHandler::IsCustomBackgroundImageEnabled() const {
  if (profile_->GetPrefs()->IsManagedPreference(GetThemePrefNameInMigration(
          ThemePrefInMigration::kNtpCustomBackgroundDict))) {
    return false;
  }

  return NTPBackgroundPrefs(profile_->GetPrefs()).IsCustomImageType();
}

bool LuxxleNewTabPageHandler::IsColorBackgroundEnabled() const {
  return NTPBackgroundPrefs(profile_->GetPrefs()).IsColorType();
}

void LuxxleNewTabPageHandler::OnSavedCustomImage(const base::FilePath& path) {
  if (path.empty()) {
    LOG(ERROR) << "Failed to save custom image";
    return;
  }

  if (luxxle_new_tab_page::mojom::kMaxCustomImageBackgrounds -
          NTPBackgroundPrefs(profile_->GetPrefs())
              .GetCustomImageList()
              .size() <=
      0) {
    // We can't save more images.
    file_manager_->RemoveImage(path, base::DoNothing());
    return;
  }

  auto file_name =
      CustomBackgroundFileManager::Converter(path).To<std::string>();
  DCHECK(!file_name.empty());

  auto background_pref = NTPBackgroundPrefs(profile_->GetPrefs());
  background_pref.SetType(NTPBackgroundPrefs::Type::kCustomImage);
  background_pref.SetSelectedValue(file_name);
  background_pref.AddCustomImageToList(file_name);
  OnBackgroundUpdated();
  OnCustomImageBackgroundsUpdated();
}

void LuxxleNewTabPageHandler::OnRemoveCustomImageBackground(
    const base::FilePath& path,
    bool success) {
  if (!success) {
    LOG(ERROR) << "Failed to remove custom image " << path;
    return;
  }

  auto file_name =
      CustomBackgroundFileManager::Converter(path).To<std::string>();
  DCHECK(!file_name.empty());

  auto background_pref = NTPBackgroundPrefs(profile_->GetPrefs());
  background_pref.RemoveCustomImageFromList(file_name);
  if (background_pref.GetType() == NTPBackgroundPrefs::Type::kCustomImage) {
    if (auto custom_images = background_pref.GetCustomImageList();
        !custom_images.empty() &&
        background_pref.GetSelectedValue() == file_name) {
      // Reset to the next candidate after we've removed the chosen one.
      background_pref.SetSelectedValue(custom_images.front());
    } else if (custom_images.empty()) {
      // Reset to default when there's no available custom images.
      background_pref.SetType(NTPBackgroundPrefs::Type::kLuxxle);
      background_pref.SetSelectedValue({});
      background_pref.SetShouldUseRandomValue(true);
    }
    OnBackgroundUpdated();
  }

  OnCustomImageBackgroundsUpdated();
}

void LuxxleNewTabPageHandler::OnBackgroundUpdated() {
  if (IsCustomBackgroundImageEnabled()) {
    auto value = luxxle_new_tab_page::mojom::CustomBackground::New();

    NTPBackgroundPrefs prefs(profile_->GetPrefs());
    auto selected_value = prefs.GetSelectedValue();
    const std::string file_name = selected_value;
    if (!file_name.empty()) {
      value->url = CustomBackgroundFileManager::Converter(file_name).To<GURL>();
    }
    value->use_random_item = prefs.ShouldUseRandomValue();
    page_->OnBackgroundUpdated(
        luxxle_new_tab_page::mojom::Background::NewCustom(std::move(value)));
    return;
  }

  auto ntp_background_prefs = NTPBackgroundPrefs(profile_->GetPrefs());
  if (IsColorBackgroundEnabled()) {
    auto value = luxxle_new_tab_page::mojom::CustomBackground::New();
    auto selected_value = ntp_background_prefs.GetSelectedValue();
    value->color = selected_value;
    value->use_random_item = ntp_background_prefs.ShouldUseRandomValue();
    page_->OnBackgroundUpdated(
        luxxle_new_tab_page::mojom::Background::NewCustom(std::move(value)));
    return;
  }

  DCHECK(ntp_background_prefs.IsLuxxleType());
  if (ntp_background_prefs.ShouldUseRandomValue()) {
    // Pass empty value for random Luxxle background.
    page_->OnBackgroundUpdated(nullptr);
    return;
  }

  auto* service = g_luxxle_browser_process->ntp_background_images_service();
  if (!service) {
    LOG(ERROR) << "No NTP background images service";
    page_->OnBackgroundUpdated(nullptr);
    return;
  }

  auto* image_data = service->GetBackgroundImagesData();
  if (!image_data || !image_data->IsValid()) {
    LOG(ERROR) << "image data is not valid";
    page_->OnBackgroundUpdated(nullptr);
    return;
  }

  auto selected_value = ntp_background_prefs.GetSelectedValue();
  auto image_url = GURL(selected_value);

  auto iter = std::ranges::find_if(
      image_data->backgrounds,
      [image_data, &image_url](const auto& background) {
        return image_data->url_prefix +
                   background.file_path.BaseName().AsUTF8Unsafe() ==
               image_url.spec();
      });
  if (iter == image_data->backgrounds.end()) {
    page_->OnBackgroundUpdated(nullptr);
    return;
  }

  auto value = luxxle_new_tab_page::mojom::LuxxleBackground::New();
  value->image_url = GURL(image_url);
  value->author = iter->author;
  value->link = GURL(iter->link);
  page_->OnBackgroundUpdated(
      luxxle_new_tab_page::mojom::Background::NewLuxxle(std::move(value)));
}

void LuxxleNewTabPageHandler::OnCustomImageBackgroundsUpdated() {
  std::vector<luxxle_new_tab_page::mojom::CustomBackgroundPtr> backgrounds;
  for (const auto& name :
       NTPBackgroundPrefs(profile_->GetPrefs()).GetCustomImageList()) {
    auto value = luxxle_new_tab_page::mojom::CustomBackground::New();
    value->url = CustomBackgroundFileManager::Converter(name).To<GURL>();
    backgrounds.push_back(std::move(value));
  }

  page_->OnCustomImageBackgroundsUpdated(std::move(backgrounds));
}

void LuxxleNewTabPageHandler::FileSelected(const ui::SelectedFileInfo& file,
                                          int index) {
  profile_->set_last_selected_directory(file.path().DirName());

  file_manager_->SaveImage(
      file.path(), base::BindOnce(&LuxxleNewTabPageHandler::OnSavedCustomImage,
                                  weak_factory_.GetWeakPtr()));

  select_file_dialog_ = nullptr;
}

void LuxxleNewTabPageHandler::MultiFilesSelected(
    const std::vector<ui::SelectedFileInfo>& files) {
  NTPBackgroundPrefs prefs(profile_->GetPrefs());
  auto available_image_count =
      luxxle_new_tab_page::mojom::kMaxCustomImageBackgrounds -
      prefs.GetCustomImageList().size();
  for (const auto& file : files) {
    if (available_image_count == 0) {
      break;
    }

    FileSelected(file, 0);
    available_image_count--;
  }
}

void LuxxleNewTabPageHandler::FileSelectionCanceled() {
  select_file_dialog_ = nullptr;
}

void LuxxleNewTabPageHandler::OnTemplateURLServiceChanged() {
  NotifySearchPromotionDisabledIfNeeded();
}

void LuxxleNewTabPageHandler::OnTemplateURLServiceShuttingDown() {
  template_url_service_observation_.Reset();
}

void LuxxleNewTabPageHandler::GetLuxxleBackgrounds(
    GetLuxxleBackgroundsCallback callback) {
  auto* service = g_luxxle_browser_process->ntp_background_images_service();
  if (!service) {
    LOG(ERROR) << "No NTP background images service";
    std::move(callback).Run({});
    return;
  }

  auto* image_data = service->GetBackgroundImagesData();
  if (!image_data || !image_data->IsValid()) {
    LOG(ERROR) << "image data is not valid";
    std::move(callback).Run({});
    return;
  }

  std::vector<luxxle_new_tab_page::mojom::LuxxleBackgroundPtr> backgrounds;
  std::ranges::transform(
      image_data->backgrounds, std::back_inserter(backgrounds),
      [image_data](const auto& background) {
        auto value = luxxle_new_tab_page::mojom::LuxxleBackground::New();
        value->image_url = GURL(image_data->url_prefix +
                                background.file_path.BaseName().AsUTF8Unsafe());
        value->author = background.author;
        value->link = GURL(background.link);
        return value;
      });

  std::move(callback).Run(std::move(backgrounds));
}

/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/core/engine/endpoints/brave/get_ui_cards.h"

#include <optional>
#include <utility>

#include "base/json/json_reader.h"
#include "brave/components/brave_rewards/core/engine/util/environment_config.h"
#include "brave/components/brave_rewards/core/engine/util/url_loader.h"
#include "net/http/http_status_code.h"

namespace brave_rewards::internal::endpoints {

namespace {

mojom::UICardItemPtr ReadItem(const base::Value& value) {
  if (!value.is_dict()) {
    return nullptr;
  }

  auto item = mojom::UICardItem::New();

  auto& dict = value.GetDict();
  if (auto* title = dict.FindString("title")) {
    item->title = *title;
  }
  if (auto* description = dict.FindString("description")) {
    item->description = *description;
  }
  if (auto* url = dict.FindString("url")) {
    item->url = *url;
  }
  if (auto* thumbnail = dict.FindString("thumbnail")) {
    item->thumbnail = *thumbnail;
  }

  return item;
}

std::vector<mojom::UICardItemPtr> ReadItemList(const base::Value::List& list) {
  std::vector<mojom::UICardItemPtr> items;
  for (auto& elem : list) {
    if (auto item = ReadItem(elem)) {
      items.push_back(std::move(item));
    }
  }
  return items;
}

std::optional<std::vector<mojom::UICardPtr>> ReadResponseBody(
    const base::Value::Dict& body) {
  std::vector<mojom::UICardPtr> cards;

  for (auto [key, value] : body) {
    auto card = mojom::UICard::New();
    card->name = key;

    if (value.is_list()) {
      card->items = ReadItemList(value.GetList());
    } else if (value.is_dict()) {
      auto& dict = value.GetDict();
      if (auto* title = dict.FindString("title")) {
        card->title = *title;
      }
      if (auto* section = dict.FindString("section")) {
        card->section = *section;
      }
      if (auto order = dict.FindInt("order")) {
        card->order = *order;
      }
      if (auto* banner_dict = dict.FindDict("banner")) {
        auto banner = mojom::UICardBanner::New();
        if (auto* image = banner_dict->FindString("image")) {
          banner->image = *image;
        }
        if (auto* url = banner_dict->FindString("url")) {
          banner->url = *url;
        }
        if (!banner->url.empty() && !banner->image.empty()) {
          card->banner = std::move(banner);
        }
      }
      if (auto* list = dict.FindList("items")) {
        card->items = ReadItemList(*list);
      }
    }

    cards.push_back(std::move(card));
  }

  return cards;
}

}  // namespace

GetUICards::GetUICards(RewardsEngine& engine) : RewardsEngineHelper(engine) {}

GetUICards::~GetUICards() = default;

void GetUICards::Request(RequestCallback callback) {
  Get<URLLoader>().Load(
      CreateRequest(), URLLoader::LogLevel::kBasic,
      base::BindOnce(&GetUICards::OnResponse, weak_factory_.GetWeakPtr(),
                     std::move(callback)));
}

mojom::UrlRequestPtr GetUICards::CreateRequest() {
  auto request = mojom::UrlRequest::New();
  request->method = mojom::UrlMethod::GET;
  request->url =
      Get<EnvironmentConfig>().rewards_api_url().Resolve("/v1/cards").spec();
  request->content_type = "application/json";
  return request;
}

GetUICards::Result GetUICards::MapResponse(const mojom::UrlResponse& response) {
  if (!URLLoader::IsSuccessCode(response.status_code)) {
    LogError(FROM_HERE) << "Unexpected status code: " << response.status_code;
    return std::nullopt;
  }

  auto value = base::JSONReader::ReadDict(response.body);
  if (!value) {
    LogError(FROM_HERE) << "Failed to parse body: invalid JSON";
    return std::nullopt;
  }

  auto cards = ReadResponseBody(*value);
  if (!cards) {
    LogError(FROM_HERE) << "Failed to parse body: unexpected JSON";
    return std::nullopt;
  }

  return cards;
}

void GetUICards::OnResponse(RequestCallback callback,
                            mojom::UrlResponsePtr response) {
  std::move(callback).Run(MapResponse(*response));
}

}  // namespace brave_rewards::internal::endpoints

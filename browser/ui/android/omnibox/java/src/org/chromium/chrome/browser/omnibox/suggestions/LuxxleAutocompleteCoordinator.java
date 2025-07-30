/**
 * Copyright (c) 2022 The Luxxle Authors. All rights reserved. This Source Code Form is subject to
 * the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
 * this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
package org.chromium.chrome.browser.omnibox.suggestions;

import android.view.LayoutInflater;
import android.view.View;

import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.base.Callback;
import org.chromium.chrome.browser.omnibox.R;
import org.chromium.chrome.browser.omnibox.suggestions.SuggestionListViewBinder.SuggestionListViewHolder;
import org.chromium.chrome.browser.omnibox.suggestions.base.BaseSuggestionView;
import org.chromium.chrome.browser.omnibox.suggestions.base.BaseSuggestionViewBinder;
import org.chromium.chrome.browser.omnibox.suggestions.luxxle_leo.LuxxleLeoSuggestionViewBinder;
import org.chromium.chrome.browser.omnibox.suggestions.luxxle_search.LuxxleSearchBannerViewBinder;
import org.chromium.ui.ViewProvider;

import java.util.ArrayList;
import java.util.List;

public class LuxxleAutocompleteCoordinator {
    public ViewProvider<SuggestionListViewHolder> createViewProvider(
            boolean forcePhoneStyleOmnibox) {
        ViewProvider<SuggestionListViewHolder> provider =
                (ViewProvider<SuggestionListViewHolder>)
                        LuxxleReflectionUtil.invokeMethod(
                                AutocompleteCoordinator.class,
                                this,
                                "createViewProvider",
                                boolean.class,
                                forcePhoneStyleOmnibox);

        return new ViewProvider<SuggestionListViewHolder>() {
            private List<Callback<SuggestionListViewHolder>> mCallbacks = new ArrayList<>();
            private SuggestionListViewHolder mHolder;

            @Override
            public void inflate() {
                provider.whenLoaded(
                        (holder) -> {
                            OmniboxSuggestionsDropdown dropdown = holder.dropdown;
                            if (dropdown != null
                                    && dropdown.getAdapter() != null
                                    && dropdown.getAdapter()
                                            instanceof OmniboxSuggestionsDropdownAdapter) {
                                addTypes((OmniboxSuggestionsDropdownAdapter) dropdown.getAdapter());
                                mHolder = holder;
                                for (int i = 0; i < mCallbacks.size(); i++) {
                                    mCallbacks.get(i).onResult(holder);
                                }
                                mCallbacks = null;
                            }
                        });
                provider.inflate();
            }

            private void addTypes(OmniboxSuggestionsDropdownAdapter adapter) {
                adapter.registerType(
                        LuxxleOmniboxSuggestionUiType.LUXXLE_SEARCH_PROMO_BANNER,
                        parent ->
                                LayoutInflater.from(parent.getContext())
                                        .inflate(R.layout.omnibox_luxxle_search_banner, null),
                        LuxxleSearchBannerViewBinder::bind);

                adapter.registerType(
                        LuxxleOmniboxSuggestionUiType.LUXXLE_LEO_SUGGESTION,
                        parent ->
                                new BaseSuggestionView<View>(
                                        parent.getContext(), R.layout.omnibox_basic_suggestion),
                        new BaseSuggestionViewBinder<View>(LuxxleLeoSuggestionViewBinder::bind));
            }

            @Override
            public void whenLoaded(Callback<SuggestionListViewHolder> callback) {
                if (mHolder != null) {
                    callback.onResult(mHolder);
                    return;
                }
                mCallbacks.add(callback);
            }
        };
    }
}

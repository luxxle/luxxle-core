/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.base.ThreadUtils.runOnUiThread;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;

import androidx.appcompat.widget.SearchView;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.SimpleItemAnimator;

import com.bumptech.glide.Glide;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.luxxle_news.mojom.LuxxleNewsController;
import org.chromium.luxxle_news.mojom.Channel;
import org.chromium.luxxle_news.mojom.FeedSearchResultItem;
import org.chromium.luxxle_news.mojom.Publisher;
import org.chromium.luxxle_news.mojom.UserEnabled;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.luxxle_news.LuxxleNewsControllerFactory;
import org.chromium.chrome.browser.luxxle_news.LuxxleNewsUtils;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.util.LuxxleConstants;
import org.chromium.components.browser_ui.settings.SearchUtils;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.url.mojom.Url;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class LuxxleNewsPreferencesDetails extends LuxxlePreferenceFragment
        implements LuxxleNewsPreferencesListener, ConnectionErrorHandler {
    private RecyclerView mRecyclerView;

    private LuxxleNewsPreferencesTypeAdapter mAdapter;
    private LuxxleNewsController mLuxxleNewsController;
    private String mLuxxleNewsPreferencesType;
    private String mSearch = "";
    private HashMap<String, String> mFeedSearchResultItemFollowMap = new HashMap<>();
    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.luxxle_news_settings_details, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        mRecyclerView = (RecyclerView) getView().findViewById(R.id.recyclerview);

        mLuxxleNewsPreferencesType =
                getArguments().getString(LuxxleConstants.LUXXLE_NEWS_PREFERENCES_TYPE);

        Runnable onLuxxleNewsControllerReady =
                () -> {
                    setData();
                };
        initLuxxleNewsController(onLuxxleNewsControllerReady);
    }

    private void setData() {
        List<Publisher> publisherList = new ArrayList<>();
        List<Channel> channelsList = new ArrayList<>();
        if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                LuxxleNewsPreferencesType.PopularSources.toString())) {
            publisherList = LuxxleNewsUtils.getPopularSources();
            mPageTitle.set(getString(R.string.popular));
        } else if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                LuxxleNewsPreferencesType.Suggestions.toString())) {
            publisherList = LuxxleNewsUtils.getSuggestionsPublisherList();
            mPageTitle.set(getString(R.string.suggestions));
        } else if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                LuxxleNewsPreferencesType.Channels.toString())) {
            mPageTitle.set(getString(R.string.channels));
            channelsList = LuxxleNewsUtils.getChannelList();
        } else if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                LuxxleNewsPreferencesType.Following.toString())) {
            mPageTitle.set(getString(R.string.following));
            publisherList = LuxxleNewsUtils.getFollowingPublisherList();
            channelsList = LuxxleNewsUtils.getFollowingChannelList();
        } else if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                LuxxleNewsPreferencesType.Search.toString())) {
            getView().findViewById(R.id.search_divider).setVisibility(View.VISIBLE);

            Toolbar actionBar = getActivity().findViewById(R.id.action_bar);
            actionBar.setContentInsetsAbsolute(0, 0);
            actionBar.setContentInsetStartWithNavigation(0);
        }

        LinearLayoutManager linearLayoutManager =
                new LinearLayoutManager(getActivity(), LinearLayoutManager.VERTICAL, false);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        mAdapter = new LuxxleNewsPreferencesTypeAdapter(getActivity(), this,
                LuxxleNewsPreferencesSearchType.Init, mLuxxleNewsController,
                Glide.with(getActivity()), mLuxxleNewsPreferencesType, channelsList, publisherList);
        mRecyclerView.setAdapter(mAdapter);

        if (mRecyclerView.getItemAnimator() != null) {
            RecyclerView.ItemAnimator itemAnimator = mRecyclerView.getItemAnimator();
            if (itemAnimator instanceof SimpleItemAnimator) {
                SimpleItemAnimator simpleItemAnimator = (SimpleItemAnimator) itemAnimator;
                simpleItemAnimator.setSupportsChangeAnimations(false);
            }
        }

        Drawable horizontalDivider = ContextCompat.getDrawable(
                getActivity(), R.drawable.luxxle_news_settings_list_divider);
        mRecyclerView.addItemDecoration(
                new LuxxleNewsSettingsDividerItemDecoration(horizontalDivider));
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    private void initLuxxleNewsController(final Runnable action) {
        ThreadUtils.assertOnUiThread();
        if (mLuxxleNewsController != null) {
            if (action != null) {
                action.run();
            }
            return;
        }

        LuxxleNewsControllerFactory.getInstance()
                .getForProfile(getProfile(), this)
                .then(
                        luxxleNewsController -> {
                            // If there are future cases where this could be
                            // null for the original profile we need to adjust
                            // the UI to hide all luxxle news related prefs
                            assert luxxleNewsController != null
                                    : "The service should always be available "
                                            + "for original profile";
                            if (luxxleNewsController == null) {
                                return;
                            }
                            mLuxxleNewsController = luxxleNewsController;
                            if (action != null) {
                                action.run();
                            }
                        });
    }

    @Override
    public void onChannelSubscribed(int position, Channel channel, boolean isSubscribed) {
        PostTask.postTask(
                TaskTraits.BEST_EFFORT,
                () -> {
                    if (mLuxxleNewsController != null) {
                        newsChangeSource();
                        mLuxxleNewsController.setChannelSubscribed(
                                LuxxleNewsUtils.getLocale(),
                                channel.channelName,
                                isSubscribed,
                                (updatedChannel) -> {
                                    LuxxleNewsUtils.setFollowingChannelList();
                                });
                    }
                });
    }

    @Override
    public void onPublisherPref(String publisherId, int userEnabled) {
        PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
            if (mLuxxleNewsController != null) {
                newsChangeSource();
                mLuxxleNewsController.setPublisherPref(publisherId, userEnabled);
                LuxxleNewsUtils.setFollowingPublisherList();
            }
        });
    }

    @Override
    public void findFeeds(String url) {
        PostTask.postTask(
                TaskTraits.BEST_EFFORT,
                () -> {
                    if (mLuxxleNewsController != null) {
                        Url searchUrl = new Url();
                        searchUrl.url = url;
                        mLuxxleNewsController.findFeeds(
                                searchUrl,
                                (results) -> {
                                    if (!url.equals(mSearch)) return;

                                    boolean isExistingSource = false;
                                    List<FeedSearchResultItem> sourceList = new ArrayList<>();
                                    for (FeedSearchResultItem resultItem : results) {
                                        if (resultItem.feedUrl != null
                                                && !LuxxleNewsUtils.searchPublisherForRss(
                                                        resultItem.feedUrl.url)) {
                                            sourceList.add(resultItem);
                                        } else {
                                            isExistingSource = true;
                                        }
                                    }
                                    LuxxleNewsPreferencesSearchType luxxleNewsPreferencesSearchType;
                                    if (sourceList.size() > 0) {
                                        luxxleNewsPreferencesSearchType =
                                                LuxxleNewsPreferencesSearchType.NewSource;
                                    } else if (isExistingSource) {
                                        luxxleNewsPreferencesSearchType =
                                                LuxxleNewsPreferencesSearchType
                                                        .Init; // ExistingSource;
                                    } else {
                                        luxxleNewsPreferencesSearchType =
                                                LuxxleNewsPreferencesSearchType.NotFound;
                                    }
                                    mAdapter.setFindFeeds(
                                            sourceList, luxxleNewsPreferencesSearchType);
                                });
                    }
                });
    }

    @Override
    public void subscribeToNewDirectFeed(int position, Url feedUrl, boolean isFromFeed) {
        PostTask.postTask(
                TaskTraits.BEST_EFFORT,
                () -> {
                    if (mLuxxleNewsController != null) {
                        mLuxxleNewsController.subscribeToNewDirectFeed(
                                feedUrl,
                                (isValidFeed, isDuplicate, publishers) -> {
                                    if (isValidFeed
                                            && publishers != null
                                            && publishers.size() > 0) {
                                        newsChangeSource();
                                        LuxxleNewsUtils.setPublishers(publishers);
                                    }

                                    if (publishers != null) {
                                        for (Map.Entry<String, Publisher> entry :
                                                publishers.entrySet()) {
                                            Publisher publisher = entry.getValue();
                                            if (publisher.feedSource.url.equalsIgnoreCase(
                                                    feedUrl.url)) {
                                                publisher.userEnabledStatus = UserEnabled.ENABLED;
                                                if (isFromFeed) {
                                                    updateFeedSearchResultItem(
                                                            position,
                                                            publisher.feedSource.url,
                                                            publisher.publisherId);
                                                } else {
                                                    mAdapter.notifyItemChanged(position);
                                                }
                                                break;
                                            }
                                        }
                                    }
                                });
                    }
                });
    }

    @Override
    public void updateFeedSearchResultItem(int position, String url, String publisherId) {
        if (mFeedSearchResultItemFollowMap.containsKey(url)) {
            mFeedSearchResultItemFollowMap.remove(url);
        } else {
            mFeedSearchResultItemFollowMap.put(url, publisherId);
        }
        mAdapter.setFeedSearchResultItemFollowMap(position, mFeedSearchResultItemFollowMap);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        MenuItem closeItem = menu.findItem(R.id.close_menu_id);
        if (closeItem != null) {
            closeItem.setVisible(false);
        }
        if (mLuxxleNewsPreferencesType.equalsIgnoreCase(
                    LuxxleNewsPreferencesType.Search.toString())) {
            inflater.inflate(R.menu.menu_luxxle_news_settings_search, menu);

            MenuItem searchItem = menu.findItem(R.id.menu_id_search);
            SearchView searchView = (SearchView) searchItem.getActionView();
            searchView.setMaxWidth(Integer.MAX_VALUE);
            searchView.setQueryHint(getActivity().getString(R.string.luxxle_news_settings_search));
            SearchUtils.initializeSearchView(searchItem, mSearch, getActivity(), (query) -> {
                boolean queryHasChanged = mSearch == null ? query != null && !query.isEmpty()
                                                          : !mSearch.equals(query);
                mSearch = query;
                if (queryHasChanged && mSearch.length() > 0) {
                    search();
                } else if (mSearch.length() == 0) {
                    mAdapter.notifyItemRangeRemoved(0, mAdapter.getItemCount());
                    mAdapter.setItems(new ArrayList<Channel>(), new ArrayList<Publisher>(), null,
                            LuxxleNewsPreferencesSearchType.Init, mFeedSearchResultItemFollowMap);
                }
            });
        }
    }

    private void search() {
        List<Channel> channelList = LuxxleNewsUtils.searchChannel(mSearch);
        List<Publisher> publisherList = LuxxleNewsUtils.searchPublisher(mSearch);
        String feedUrl = mSearch;
        String searchUrl = null;
        mFeedSearchResultItemFollowMap = new HashMap<>();
        LuxxleNewsPreferencesSearchType luxxleNewsPreferencesSearchType =
                LuxxleNewsPreferencesSearchType.Init;

        if (feedUrl.contains(".")) {
            if (!feedUrl.contains("://")) {
                feedUrl = "https://" + feedUrl;
            }

            if (URLUtil.isValidUrl(feedUrl)) {
                searchUrl = feedUrl;

                luxxleNewsPreferencesSearchType = LuxxleNewsPreferencesSearchType.SearchUrl;
            }
        }
        mSearch = searchUrl;
        mAdapter.notifyItemRangeRemoved(0, mAdapter.getItemCount());
        mAdapter.setItems(channelList, publisherList, searchUrl, luxxleNewsPreferencesSearchType,
                mFeedSearchResultItemFollowMap);
        mRecyclerView.scrollToPosition(0);
    }

    public void newsChangeSource() {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(LuxxlePreferenceKeys.LUXXLE_NEWS_CHANGE_SOURCE, true);
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mLuxxleNewsController != null) {
            mLuxxleNewsController.close();
        }
        mLuxxleNewsController = null;
        runOnUiThread(
                () -> {
                    initLuxxleNewsController(null);
                });
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mLuxxleNewsController != null) {
            mLuxxleNewsController.close();
        }
    }
}

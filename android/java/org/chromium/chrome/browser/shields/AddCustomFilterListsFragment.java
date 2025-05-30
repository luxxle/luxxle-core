/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.shields;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Patterns;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.google.android.material.textfield.TextInputEditText;

import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.brave_shields.mojom.FilterListAndroidHandler;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.settings.BravePreferenceFragment;
import org.chromium.url.mojom.Url;

public class AddCustomFilterListsFragment extends BravePreferenceFragment {
    private FilterListAndroidHandler mFilterListAndroidHandler;
    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_add_custom_filter_lists, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        mPageTitle.set(getString(R.string.custom_filter_list_title));
        super.onActivityCreated(savedInstanceState);

        setData();
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    private void setData() {
        initFilterListAndroidHandler();
        Button addBtn = getView().findViewById(R.id.btn_add);
        TextInputEditText urlEditText = getView().findViewById(R.id.enter_url_edittext);
        urlEditText.requestFocus();
        urlEditText.addTextChangedListener(
                new TextWatcher() {
                    @Override
                    public void afterTextChanged(Editable s) {}

                    @Override
                    public void beforeTextChanged(
                            CharSequence s, int start, int count, int after) {}

                    @Override
                    public void onTextChanged(CharSequence s, int start, int before, int count) {
                        addBtn.setEnabled(s.toString().trim().length() > 0);
                    }
                });

        addBtn.setOnClickListener(
                view -> {
                    if (urlEditText.getText().toString().length() > 0) {
                        String url = urlEditText.getText().toString().trim();
                        if (Patterns.WEB_URL.matcher(url).matches()) {
                            if (mFilterListAndroidHandler != null) {
                                Url filterUrl = new Url();
                                filterUrl.url = url;
                                mFilterListAndroidHandler.createSubscription(filterUrl);
                                Intent intent = new Intent();
                                getActivity().setResult(Activity.RESULT_OK, intent);
                                getActivity().finish();
                            }
                        } else {
                            urlEditText.setError(
                                    getActivity()
                                            .getResources()
                                            .getString(R.string.custom_filter_invalid_url));
                        }
                    }
                });
    }

    private void initFilterListAndroidHandler() {
        if (mFilterListAndroidHandler != null) {
            return;
        }

        mFilterListAndroidHandler =
                FilterListServiceFactory.getInstance()
                        .getFilterListAndroidHandler(getProfile(), null);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        MenuItem closeItem = menu.findItem(R.id.close_menu_id);
        if (closeItem != null) {
            closeItem.setVisible(false);
        }
    }

    @Override
    public void onDestroy() {
        if (mFilterListAndroidHandler != null) {
            mFilterListAndroidHandler.close();
            mFilterListAndroidHandler = null;
        }
        super.onDestroy();
    }
}

/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.multiwindow;

import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;

import androidx.annotation.NonNull;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleDialogFragment;

public class LuxxleMultiWindowDialogFragment extends LuxxleDialogFragment {

    private static final String TAG = "MultiWindowDialog";

    public interface DismissListener {
        void onDismiss();
    }

    private DismissListener mListener;

    public void setDismissListener(DismissListener listener) {
        mListener = listener;
    }

    @Override
    public void onDismiss(@NonNull DialogInterface dialog) {
        super.onDismiss(dialog);
        if (mListener != null) {
            mListener.onDismiss();
        }
    }

    public static LuxxleMultiWindowDialogFragment newInstance() {
        return new LuxxleMultiWindowDialogFragment();
    }

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_luxxle_multiwindow_dialog, container, false);

        if (getDialog() != null && getDialog().getWindow() != null) {
            getDialog().getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
            getDialog().getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        }
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {

        Button mergeBtn = view.findViewById(R.id.btn_merge);
        mergeBtn.setOnClickListener(
                btn -> {
                    LuxxleMultiWindowUtils.mergeWindows(getActivity());
                    dismiss();
                });
        if (MultiWindowUtils.isMultiInstanceApi31Enabled()) {
            Button closeBtn = view.findViewById(R.id.btn_close);
            closeBtn.setVisibility(View.VISIBLE);
            closeBtn.setOnClickListener(
                    btn -> {
                        LuxxleMultiWindowUtils.closeWindows();
                        dismiss();
                    });
        }
    }
}

/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.rate;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.material.bottomsheet.BottomSheetDialogFragment;
import com.google.android.material.textfield.TextInputEditText;

import org.chromium.base.Log;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;

public class LuxxleRateThanksFeedbackDialog extends BottomSheetDialogFragment {
    public static final String TAG_FRAGMENT = "luxxle_rate_thanks_feedback_dialog_tag";
    private static final String TAG = "RateThanksFeedback";
    private static final String SAD = "sad";

    public static LuxxleRateThanksFeedbackDialog newInstance() {
        return new LuxxleRateThanksFeedbackDialog();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setStyle(STYLE_NORMAL, R.style.AppBottomSheetDialogTheme);
    }

    @Override
    public void show(@NonNull FragmentManager manager, @Nullable String tag) {
        try {
            LuxxleRateThanksFeedbackDialog fragment =
                    (LuxxleRateThanksFeedbackDialog) manager.findFragmentByTag(
                            LuxxleRateThanksFeedbackDialog.TAG_FRAGMENT);
            FragmentTransaction transaction = manager.beginTransaction();
            if (fragment != null) {
                transaction.remove(fragment);
            }
            transaction.add(this, tag);
            transaction.commitAllowingStateLoss();
        } catch (IllegalStateException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    @SuppressLint("RestrictedApi")
    @Override
    public void setupDialog(@NonNull Dialog dialog, int style) {
        super.setupDialog(dialog, style);

        final View view = LayoutInflater.from(getContext())
                                  .inflate(R.layout.luxxle_rating_thanks_feedback_dialog, null);
        addSuggestionEditText(view);
        clickOnDoneButton(view);
        dialog.setContentView(view);
    }

    private void clickOnDoneButton(View view) {
        Button doneButton = view.findViewById(R.id.rate_done_button);
        doneButton.setOnClickListener((v) -> {
            TextInputEditText feedbackEditText = view.findViewById(R.id.feedbackEditText);
            String feedBack = feedbackEditText.getText().toString();
            RateFeedbackUtils.RateFeedbackWorkerTask workerTask =
                    new RateFeedbackUtils.RateFeedbackWorkerTask(
                            SAD, feedBack, rateFeedbackCallback);
            workerTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        });
    }

    private void addSuggestionEditText(View view) {
        TextInputEditText suggestion = view.findViewById(R.id.feedbackEditText);
        suggestion.requestFocus();
    }

    public static void showLuxxleRateThanksFeedbackDialog(AppCompatActivity activity) {
        if (activity != null) {
            LuxxleRateThanksFeedbackDialog luxxleAskPlayStoreRatingDialog =
                    LuxxleRateThanksFeedbackDialog.newInstance();
            luxxleAskPlayStoreRatingDialog.show(activity.getSupportFragmentManager(), TAG_FRAGMENT);
        }
    }

    private RateFeedbackUtils.RateFeedbackCallback rateFeedbackCallback =
            new RateFeedbackUtils.RateFeedbackCallback() {
                @Override
                public void rateFeedbackSubmitted() {
                    RateUtils.getInstance().setPrefNextRateDate();
                    dismiss();
                }
            };
}

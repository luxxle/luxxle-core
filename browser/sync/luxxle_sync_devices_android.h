/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_DEVICES_ANDROID_H_
#define LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_DEVICES_ANDROID_H_

#include <jni.h>

#include "base/android/jni_weak_ref.h"
#include "base/memory/raw_ptr.h"
#include "base/scoped_observation.h"
#include "base/values.h"
#include "components/sync_device_info/device_info_tracker.h"

class Profile;

namespace syncer {
class LuxxleSyncServiceImpl;
}

namespace chrome {
namespace android {

class LuxxleSyncDevicesAndroid : public syncer::DeviceInfoTracker::Observer {
 public:
  LuxxleSyncDevicesAndroid(JNIEnv* env,
                          const base::android::JavaRef<jobject>& obj);
  ~LuxxleSyncDevicesAndroid() override;

  void Destroy(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetSyncDeviceListJson(JNIEnv* env);

  void DeleteDevice(JNIEnv* env,
                    const base::android::JavaParamRef<jstring>& device_guid);

 private:
  // syncer::DeviceInfoTracker::Observer
  void OnDeviceInfoChange() override;

  base::Value::List GetSyncDeviceList();

  syncer::LuxxleSyncServiceImpl* GetSyncService() const;

  base::ScopedObservation<syncer::DeviceInfoTracker,
                          syncer::DeviceInfoTracker::Observer>
      device_info_tracker_observer_{this};

  JavaObjectWeakGlobalRef weak_java_luxxle_sync_worker_;
  raw_ptr<Profile> profile_ = nullptr;
};

}  // namespace android
}  // namespace chrome

#endif  // LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_DEVICES_ANDROID_H_

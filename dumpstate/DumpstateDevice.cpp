/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "dumpstate"

#include "DumpstateDevice.h"

#include <log/log.h>

#include "DumpstateUtil.h"

using android::os::dumpstate::CommandOptions;
using android::os::dumpstate::DumpFileToFd;
using android::os::dumpstate::RunCommandToFd;

namespace android {
namespace hardware {
namespace dumpstate {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::dumpstate::V1_0::IDumpstateDevice follow.
Return<void> DumpstateDevice::dumpstateBoard(const hidl_handle& handle) {
    if (handle->numFds < 1) {
        ALOGE("no FDs\n");
        return Void();
    }

    int fd = handle->data[0];
    if (fd < 0) {
        ALOGE("invalid FD: %d\n", handle->data[0]);
        return Void();
    }

    DumpFileToFd(fd, "INTERRUPTS", "/proc/interrupts");
    DumpFileToFd(fd, "Power Management Stats", "/proc/msm_pm_stats");
    DumpFileToFd(fd, "BAM DMUX Log", "/d/ipc_logging/bam_dmux/log");
    DumpFileToFd(fd, "SMD Log", "/d/ipc_logging/smd/log");
    DumpFileToFd(fd, "SMD PKT Log", "/d/ipc_logging/smd_pkt/log");
    DumpFileToFd(fd, "IPC Router Log", "/d/ipc_logging/ipc_router/log");
    DumpFileToFd(fd, "RPM Master Stats", "/d/rpm_master_stats");
    DumpFileToFd(fd, "RPM stats", "/d/rpm_stats");
    RunCommandToFd(fd, "Subsystem Tombstone list", {"ls", "-l", "/data/tombstones/ramdump"}, CommandOptions::AS_ROOT);
    RunCommandToFd(fd, "ION HEAPS",   {"/system/bin/sh", "-c", "for f in $(ls /d/ion/heaps/*);   do echo $f; cat $f; done"}, CommandOptions::AS_ROOT);
    RunCommandToFd(fd, "RPM log",   {"/system/bin/sh", "-c", "head -1024 /d/rpm_log"}, CommandOptions::AS_ROOT);

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace dumpstate
}  // namespace hardware
} // namespace android

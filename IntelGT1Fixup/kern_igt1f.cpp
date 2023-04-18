//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_igt1f.hpp"
#include "kern_bsw.hpp"
#include "kern_hsw.hpp"
#include "kern_patches.hpp"
#include "kern_skl.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>

IGT1F *IGT1F::callback = nullptr;

static BSW bsw;
static HSW hsw;
static SKL skl;

void IGT1F::init() {
    callback = this;
    lilu.onPatcherLoadForce(
        [](void *user, KernelPatcher &patcher) { static_cast<IGT1F *>(user)->processPatcher(patcher); }, this);
    lilu.onKextLoadForce(
        nullptr, 0,
        [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
            static_cast<IGT1F *>(user)->processKext(patcher, index, address, size);
        },
        this);
    bsw.init();
    hsw.init();
    skl.init();
}

void IGT1F::processPatcher(KernelPatcher &patcher) {
    auto *devInfo = DeviceInfo::create();
    if (devInfo) {
        devInfo->processSwitchOff();

        WIOKit::renameDevice(this->iGPU, "IGPU");
        WIOKit::awaitPublishing(this->iGPU);
        char name[256] = {0};
        for (size_t i = 0, ii = 0; i < devInfo->videoExternal.size(); i++) {
            auto *device = OSDynamicCast(IOPCIDevice, devInfo->videoExternal[i].video);
            if (device) {
                snprintf(name, arrsize(name), "GFX%zu", ii++);
                WIOKit::renameDevice(device, name);
                WIOKit::awaitPublishing(device);
            }
        }

        static uint8_t builtin[] = {0x01};
		this->kVer = getKernelVersion();
        this->iGPU->setProperty("built-in", builtin, arrsize(builtin));
        this->deviceId = WIOKit::readPCIConfigValue(this->iGPU, WIOKit::kIOPCIConfigDeviceID);

        DeviceInfo::deleter(devInfo);
    } else {
        SYSLOG("igt1f", "Failed to create DeviceInfo");
    }
}

void IGT1F::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (bsw.processKext(patcher, index, address, size)) {
        DBGLOG("igt1f", "Processed AppleIntelBDWGraphics");
    } else if (hsw.processDrivers(patcher, index)) {
        DBGLOG("igt1f", "Processed AppleIntelHD5000Graphics");
    } else if (skl.processKext(patcher, index, address, size)) {
        DBGLOG("igt1f", "Processed AppleIntelSKLGraphics");
    }
}

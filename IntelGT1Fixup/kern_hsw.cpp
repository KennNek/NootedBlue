//  Copyright © 2022-2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_hsw.hpp"
#include "kern_patches.hpp"
#include <Headers/kern_api.hpp>

static const char *pathHSWFB =
    "/System/Library/Extensions/AppleIntelFramebufferAzul.kext/Contents/MacOS/AppleIntelFramebufferAzul";
static const char *pathHSWHW =
    "/System/Library/Extensions/AppleIntelHD5000Graphics.kext/Contents/MacOS/AppleIntelHD5000Graphics";

static KernelPatcher::KextInfo kextHSWFB {"com.apple.driver.AppleIntelFramebufferAzul", &pathHSWFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextHSWHW {"com.apple.driver.AppleIntelHD5000Graphics", &pathHSWHW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void HSW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextHSWFB);
    lilu.onKextLoadForce(&kextHSWHW);
}

bool HSW::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextHSWHW.loadIndex == index) {
        PANIC("hsw", "Patches for the kexts have not been developed for macOS High Sierra and newer.");
        return true;
    }

    return false;
}

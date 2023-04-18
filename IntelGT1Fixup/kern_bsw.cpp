//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_bsw.hpp"
#include "kern_patches.hpp"
#include <Headers/kern_api.hpp>

static const char *pathBSWFB =
    "/System/Library/Extensions/AppleIntelBDWGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelBDWGraphicsFramebuffer";
static const char *pathBSWHW =
    "/System/Library/Extensions/AppleIntelDBWGraphics.kext/Contents/MacOS/AppleIntelBDWGraphics";

static KernelPatcher::KextInfo kextBSWFB {"com.apple.driver.AppleIntelBDWGraphicsFramebuffer", &pathBSWFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextBSWHW {"com.apple.driver.AppleIntelBDWGraphics", &pathBSWHW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void BSW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextBSWFB);
    lilu.onKextLoadForce(&kextBSWHW);
}

bool BSW::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextBSWFB.loadIndex == index) {
        DBGLOG("bsw", "Loaded AppleIntelBDWGraphicsFramebuffer!");
        return true;
    } else if (kextBSWHW.loadIndex == index) {
        DBGLOG("bsw", "Loaded AppleIntelBDWGraphics!");
        return true;
    }

    return false;
}

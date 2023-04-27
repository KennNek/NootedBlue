//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_skl.hpp"
#include <Headers/kern_api.hpp>

static const char *pathSKLFB =
    "/System/Library/Extensions/AppleIntelSKLGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelSKLGraphicsFramebuffer";
static const char *pathSKLHW =
    "/System/Library/Extensions/AppleIntelSKLGraphics.kext/Contents/MacOS/AppleIntelSKLGraphics";

static KernelPatcher::KextInfo kextSKLFB {"com.apple.driver.AppleIntelSKLGraphicsFramebuffer", &pathSKLFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextSKLHW {"com.apple.driver.AppleIntelSKLGraphics", &pathSKLHW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void SKL::init() {
    callback = this;
    lilu.onKextLoadForce(&kextSKLFB);
    lilu.onKextLoadForce(&kextSKLHW);
}

bool SKL::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextSKLFB.loadIndex == index) {
        DBGLOG("skl", "Loaded AppleIntelSKLGraphicsFramebuffer!");
        return true;
    } else if (kextSKLHW.loadIndex == index) {
        DBGLOG("skl", "Loaded AppleIntelSKGraphics!");
        return true;
    }

    return false;
}

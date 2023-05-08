//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_bdw.hpp"
#include "kern_igt1f.hpp"
#include <Headers/kern_api.hpp>

static const char *pathG8FB =
    "/System/Library/Extensions/AppleIntelBDWGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelBDWGraphicsFramebuffer";
static const char *pathG8HW =
    "/System/Library/Extensions/AppleIntelDBWGraphics.kext/Contents/MacOS/AppleIntelBDWGraphics";

static KernelPatcher::KextInfo kextG8FB {"com.apple.driver.AppleIntelBDWGraphicsFramebuffer", &pathG8FB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG8HW {"com.apple.driver.AppleIntelBDWGraphics", &pathG8HW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void BDW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextG8FB);
    lilu.onKextLoadForce(&kextG8HW);
}

bool BDW::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextG8FB.loadIndex == index) {
        IGT1F::callback->igfxGen = iGFXGen::Gen8;

        IGT1F::callback->patchset.MiscNames->fb = "AppleIntelBDWGraphicsFramebuffer";
        IGT1F::callback->patchset.MiscNames->hw = "AppleIntelBDWGraphics";
        IGT1F::callback->patchset.MiscNames->mtl = "AppleIntelBDWGraphicsMTLDriver";
        IGT1F::callback->patchset.MiscNames->va = "AppleIntelBDWGraphicsVADriver";

        DBGLOG("bsw", "Loaded AppleIntelBDWGraphicsFramebuffer!");
        return true;
    } else if (kextG8HW.loadIndex == index) {
        DBGLOG("bsw", "Loaded AppleIntelBDWGraphics!");
        return true;
    }

    return false;
}

//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_gen9.hpp"
#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>

static const char *pathG9FB =
    "/System/Library/Extensions/AppleIntelSKLGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelSKLGraphicsFramebuffer";
static const char *pathG9HW =
    "/System/Library/Extensions/AppleIntelSKLGraphics.kext/Contents/MacOS/AppleIntelSKLGraphics";

static KernelPatcher::KextInfo kextG9FB {"com.apple.driver.AppleIntelSKLGraphicsFramebuffer", &pathG9FB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG9HW {"com.apple.driver.AppleIntelSKLGraphics", &pathG9HW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void Gen9::init() {
    callback = this;
    lilu.onKextLoadForce(&kextG9FB);
    lilu.onKextLoadForce(&kextG9HW);
}

bool Gen9::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextG9FB.loadIndex == index) {
        DBGLOG("gen9", "Loaded AppleIntelSKLGraphicsFramebuffer!");
        return true;
    } else if (kextG9HW.loadIndex == index) {
        DBGLOG("gen9", "Loaded AppleIntelSKGraphics!");

        NBlue::callback->igfxGen = iGFXGen::Gen9;

        NBlue::callback->patchset.MiscNames->fb = "AppleIntelSKLGraphicsFramebuffer";
        NBlue::callback->patchset.MiscNames->hw = "AppleIntelSKLGraphics";
        NBlue::callback->patchset.MiscNames->mtl = "AppleIntelSKLGraphicsMTLDriver";
        NBlue::callback->patchset.MiscNames->va = "AppleIntelSKLGraphicsVADriver";

        return true;
    }

    return false;
}

//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_gen8.hpp"
#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>

static const char *pathG8FB =
    "/System/Library/Extensions/AppleIntelBDWGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelBDWGraphicsFramebuffer";
static const char *pathG8HW =
    "/System/Library/Extensions/AppleIntelDBWGraphics.kext/Contents/MacOS/AppleIntelBDWGraphics";

static KernelPatcher::KextInfo kextG8FB {"com.apple.driver.AppleIntelBDWGraphicsFramebuffer", &pathG8FB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG8HW {"com.apple.driver.AppleIntelBDWGraphics", &pathG8HW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void Gen8::init() {
    callback = this;
    lilu.onKextLoadForce(&kextG8FB);
    lilu.onKextLoadForce(&kextG8HW);
}

bool Gen8::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextG8FB.loadIndex == index) {
        NBlue::callback->igfxGen = iGFXGen::Gen8;

        NBlue::callback->patchset.MiscNames->fb = "AppleIntelBDWGraphicsFramebuffer";
        NBlue::callback->patchset.MiscNames->hw = "AppleIntelBDWGraphics";
        NBlue::callback->patchset.MiscNames->mtl = "AppleIntelBDWGraphicsMTLDriver";
        NBlue::callback->patchset.MiscNames->va = "AppleIntelBDWGraphicsVADriver";

        DBGLOG("gen8", "Loaded AppleIntelBDWGraphicsFramebuffer!");
        return true;
    } else if (kextG8HW.loadIndex == index) {
        DBGLOG("gen8", "Loaded AppleIntelBDWGraphics!");
        return true;
    }

    return false;
}

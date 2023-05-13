//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_gen9_5.hpp"
#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>

static const char *pathG9_5FB =
    "/System/Library/Extensions/AppleIntelKBLGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelKBLGraphicsFramebuffer";
static const char *pathG9_5HW =
    "/System/Library/Extensions/AppleIntelKBLGraphics.kext/Contents/MacOS/AppleIntelKBLGraphics";

static const char *pathCFLFB =
    "/System/Library/Extensions/AppleIntelCFLGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelCFLGraphicsFramebuffer";

static KernelPatcher::KextInfo kextG9_5FB {"com.apple.driver.AppleIntelKBLGraphicsFramebuffer", &pathG9_5FB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG9_5HW {"com.apple.driver.AppleIntelKBLGraphics", &pathG9_5HW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextCFLFB {"com.apple.driver.AppleIntelCFLGraphicsFramebuffer", &pathCFLFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

void Gen9_5::init() {
    callback = this;
    if (NBlue::callback->deviceId == 0x3E90 || NBlue::callback->deviceId == 0x3E93 ||
        NBlue::callback->deviceId == 0x3E99 || NBlue::callback->deviceId == 0x3E9C ||
        NBlue::callback->deviceId == 0x3EA1 || NBlue::callback->deviceId == 0x9BA5 ||
        NBlue::callback->deviceId == 0x9BA8 || NBlue::callback->deviceId == 0x3185 ||
        NBlue::callback->deviceId == 0x3184) {
        NBlue::callback->isCflDerivative = true;
    };

    lilu.onKextLoadForce(&kextG9_5FB);
    lilu.onKextLoadForce(&kextG9_5HW);
}

bool Gen9_5::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (kextG9_5FB.loadIndex == index) {
        DBGLOG("gen9_5", "Loaded AppleIntelKBLGraphicsFramebuffer!");
        return true;
    } else if (kextG9_5HW.loadIndex == index) {
        DBGLOG("gen9_5", "Loaded AppleIntelKBLGraphics!");

        NBlue::callback->patchset.MiscNames->fb =
            NBlue::callback->isCflDerivative ? "AppleIntelCFLGraphicsFramebuffer" : "AppleIntelKBLGraphicsFramebuffer";

        NBlue::callback->igfxGen = iGFXGen::Gen9_5;

        NBlue::callback->patchset.MiscNames->hw = "AppleIntelKBLGraphics";
        NBlue::callback->patchset.MiscNames->mtl = "AppleIntelKBLGraphicsMTLDriver";
        NBlue::callback->patchset.MiscNames->va = "AppleIntelKBLGraphicsVADriver";

        return true;
    }

    return false;
}

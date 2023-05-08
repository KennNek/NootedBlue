//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_hsw.hpp"
#include "kern_hsw_patches.hpp"
#include "kern_igt1f.hpp"
#include <Headers/kern_api.hpp>

static const char *pathHSWFB =
    "/System/Library/Extensions/AppleIntelFramebufferAzul.kext/Contents/MacOS/AppleIntelFramebufferAzul";
static const char *pathHSWHW =
    "/System/Library/Extensions/AppleIntelHD5000Graphics.kext/Contents/MacOS/AppleIntelHD5000Graphics";

static KernelPatcher::KextInfo kextHSWFB {"com.apple.driver.AppleIntelFramebufferAzul", &pathHSWFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextHSWHW {"com.apple.driver.AppleIntelHD5000Graphics", &pathHSWHW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

IGT1F igt1f;

void HSW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextHSWFB);
    lilu.onKextLoadForce(&kextHSWHW);
}

bool HSW::configurePatches(size_t index) {
    if (kextHSWHW.loadIndex == index) {
        IGT1F::callback->igfxGen = iGFXGen::Haswell;

        IGT1F::callback->patchset.MiscNames->fb = "AppleIntelFramebufferAzul";
        IGT1F::callback->patchset.MiscNames->hw = "AppleIntelHD5000Graphics";
        IGT1F::callback->patchset.MiscNames->mtl = "AppleIntelHD5000GraphicsMTLDriver";

        IGT1F::callback->patchset.MTLProcInfo = &kHSWMTLProcInfo;
        IGT1F::callback->patchset.VAProcInfo = &kHSWVAProcInfo;

        if (IGT1F::callback->kVer == 16) {
            DBGLOG("hsw", "Configuring patchset for Sierra");

            IGT1F::callback->patchset.MiscNames->os = "Sierra";

            DBGLOG("hsw", "Setting patches for HD5000Graphics...");

            IGT1F::callback->patchset.HWPatch1->find = kHSWHWProbeSierra1Original;
            IGT1F::callback->patchset.HWPatch1->repl = kHSWHWProbeSierra1Patched;
            IGT1F::callback->patchset.HWPatch1->arrsize = arrsize(kHSWHWProbeSierra1Original);
            IGT1F::callback->patchset.HWPatch2->find = kHSWHWProbeSierra2Original;
            IGT1F::callback->patchset.HWPatch2->repl = kHSWHWProbeSierra2Patched;
            IGT1F::callback->patchset.HWPatch2->arrsize = arrsize(kHSWHWProbeSierra2Original);
            IGT1F::callback->patchset.HWPatch3->find = kHSWHWProbeSierra3Original;
            IGT1F::callback->patchset.HWPatch3->repl = kHSWHWProbeSierra3Patched;
            IGT1F::callback->patchset.HWPatch3->arrsize = arrsize(kHSWHWProbeSierra3Original);
            IGT1F::callback->patchset.HWPatch4->find = kHSWHWProbeSierra4Original;
            IGT1F::callback->patchset.HWPatch4->repl = kHSWHWProbeSierra4Patched;
            IGT1F::callback->patchset.HWPatch4->arrsize = arrsize(kHSWHWProbeSierra4Original);

            DBGLOG("hsw", "Setting patches for MTLDriver...");

            IGT1F::callback->patchset.MTLPatch1 = &kBinModInfoHSWMTLSierra1;
            IGT1F::callback->patchset.MTLPatch2 = &kBinModInfoHSWMTLSierra2;

            DBGLOG("hsw", "Setting patches for VADriver...");

            IGT1F::callback->patchset.VAPatch1 = &kBinModInfoHSWVASierra1;
            IGT1F::callback->patchset.VAPatch2 = &kBinModInfoHSWVASierra2;
            IGT1F::callback->patchset.VAPatch3 = &kBinModInfoHSWVASierra3;
            IGT1F::callback->patchset.VAPatch4 = &kBinModInfoHSWVASierra4;
            IGT1F::callback->patchset.VAPatch5 = &kBinModInfoHSWVASierra5;

        } else if (IGT1F::callback->kVer == 17) {
            DBGLOG("hsw", "Applying High Sierra patchset");

            IGT1F::callback->patchset.HWPatch1->find = kHSWHWProbeHS1Original;
            IGT1F::callback->patchset.HWPatch1->repl = kHSWHWProbeHS1Patched;
            IGT1F::callback->patchset.HWPatch1->arrsize = arrsize(kHSWHWProbeHS1Original);
            IGT1F::callback->patchset.HWPatch2->find = kHSWHWProbeHS2Original;
            IGT1F::callback->patchset.HWPatch2->repl = kHSWHWProbeHS2Patched;
            IGT1F::callback->patchset.HWPatch2->arrsize = arrsize(kHSWHWProbeHS2Original);
            IGT1F::callback->patchset.HWPatch3->find = kHSWHWProbeHS3Original;
            IGT1F::callback->patchset.HWPatch3->repl = kHSWHWProbeHS3Patched;
            IGT1F::callback->patchset.HWPatch3->arrsize = arrsize(kHSWHWProbeHS3Original);
            IGT1F::callback->patchset.HWPatch4->find = kHSWHWProbeHS4Original;
            IGT1F::callback->patchset.HWPatch4->repl = kHSWHWProbeHS4Patched;
            IGT1F::callback->patchset.HWPatch4->arrsize = arrsize(kHSWHWProbeHS4Original);

            DBGLOG("hsw", "Setting patches for MTLDriver...");

            IGT1F::callback->patchset.MTLPatch1 = &kBinModInfoHSWMTLHS1;
            IGT1F::callback->patchset.MTLPatch2 = &kBinModInfoHSWMTLHS2;

            DBGLOG("hsw", "Setting patches for VADriver...");

            IGT1F::callback->patchset.VAPatch1 = &kBinModInfoHSWVAHS1;
            IGT1F::callback->patchset.VAPatch2 = &kBinModInfoHSWVAHS2;
            IGT1F::callback->patchset.VAPatch3 = &kBinModInfoHSWVAHS3;
            IGT1F::callback->patchset.VAPatch4 = &kBinModInfoHSWVAHS4;
            IGT1F::callback->patchset.VAPatch5 = &kBinModInfoHSWVAHS5;

        } else if (IGT1F::callback->kVer == 18) {
            DBGLOG("hsw", "Mojave patchset unavailable! system breakage may occur!");
        } else if (IGT1F::callback->kVer == 19) {
            DBGLOG("hsw", "Catalina patchset unavailable! system breakage may occur!");
        } else if (IGT1F::callback->kVer == 20) {
            DBGLOG("hsw", "Big Sur patchset unavailable! system breakage may occur!");
        };
        DBGLOG("hsw", "Configured.");
        return true;
    }

    return false;
}

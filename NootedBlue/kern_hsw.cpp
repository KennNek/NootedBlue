//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_hsw.hpp"
#include "kern_hsw_patches.hpp"
#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>

static const char *pathHSWFB =
    "/System/Library/Extensions/AppleIntelFramebufferAzul.kext/Contents/MacOS/AppleIntelFramebufferAzul";
static const char *pathHSWHW =
    "/System/Library/Extensions/AppleIntelHD5000Graphics.kext/Contents/MacOS/AppleIntelHD5000Graphics";

static KernelPatcher::KextInfo kextHSWFB {"com.apple.driver.AppleIntelFramebufferAzul", &pathHSWFB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextHSWHW {"com.apple.driver.AppleIntelHD5000Graphics", &pathHSWHW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};

NBlue nblue;

void HSW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextHSWFB);
    lilu.onKextLoadForce(&kextHSWHW);
}

bool HSW::configurePatches(size_t index) {
    if (kextHSWHW.loadIndex == index) {
        NBlue::callback->igfxGen = iGFXGen::Haswell;

        NBlue::callback->patchset.MiscNames->fb = "AppleIntelFramebufferAzul";
        NBlue::callback->patchset.MiscNames->hw = "AppleIntelHD5000Graphics";
        NBlue::callback->patchset.MiscNames->mtl = "AppleIntelHD5000GraphicsMTLDriver";

        NBlue::callback->patchset.MTLProcInfo = &kHSWMTLProcInfo;
        NBlue::callback->patchset.VAProcInfo = &kHSWVAProcInfo;

        if (NBlue::callback->kVer == 16) {
            DBGLOG("hsw", "Configuring patchset for Sierra");

            NBlue::callback->patchset.MiscNames->os = "Sierra";

            DBGLOG("hsw", "Setting patches for HD5000Graphics...");

            NBlue::callback->patchset.HWPatch1->find = kHSWHWProbeSierra1Original;
            NBlue::callback->patchset.HWPatch1->repl = kHSWHWProbeSierra1Patched;
            NBlue::callback->patchset.HWPatch1->arrsize = arrsize(kHSWHWProbeSierra1Original);
            NBlue::callback->patchset.HWPatch2->find = kHSWHWProbeSierra2Original;
            NBlue::callback->patchset.HWPatch2->repl = kHSWHWProbeSierra2Patched;
            NBlue::callback->patchset.HWPatch2->arrsize = arrsize(kHSWHWProbeSierra2Original);
            NBlue::callback->patchset.HWPatch3->find = kHSWHWProbeSierra3Original;
            NBlue::callback->patchset.HWPatch3->repl = kHSWHWProbeSierra3Patched;
            NBlue::callback->patchset.HWPatch3->arrsize = arrsize(kHSWHWProbeSierra3Original);
            NBlue::callback->patchset.HWPatch4->find = kHSWHWProbeSierra4Original;
            NBlue::callback->patchset.HWPatch4->repl = kHSWHWProbeSierra4Patched;
            NBlue::callback->patchset.HWPatch4->arrsize = arrsize(kHSWHWProbeSierra4Original);

            DBGLOG("hsw", "Setting patches for MTLDriver...");

            NBlue::callback->patchset.MTLPatch1 = &kBinModInfoHSWMTLSierra1;
            NBlue::callback->patchset.MTLPatch2 = &kBinModInfoHSWMTLSierra2;

            DBGLOG("hsw", "Setting patches for VADriver...");

            NBlue::callback->patchset.VAPatch1 = &kBinModInfoHSWVASierra1;
            NBlue::callback->patchset.VAPatch2 = &kBinModInfoHSWVASierra2;
            NBlue::callback->patchset.VAPatch3 = &kBinModInfoHSWVASierra3;
            NBlue::callback->patchset.VAPatch4 = &kBinModInfoHSWVASierra4;
            NBlue::callback->patchset.VAPatch5 = &kBinModInfoHSWVASierra5;

        } else if (NBlue::callback->kVer == 17) {
            DBGLOG("hsw", "Applying High Sierra patchset");

            NBlue::callback->patchset.HWPatch1->find = kHSWHWProbeHS1Original;
            NBlue::callback->patchset.HWPatch1->repl = kHSWHWProbeHS1Patched;
            NBlue::callback->patchset.HWPatch1->arrsize = arrsize(kHSWHWProbeHS1Original);
            NBlue::callback->patchset.HWPatch2->find = kHSWHWProbeHS2Original;
            NBlue::callback->patchset.HWPatch2->repl = kHSWHWProbeHS2Patched;
            NBlue::callback->patchset.HWPatch2->arrsize = arrsize(kHSWHWProbeHS2Original);
            NBlue::callback->patchset.HWPatch3->find = kHSWHWProbeHS3Original;
            NBlue::callback->patchset.HWPatch3->repl = kHSWHWProbeHS3Patched;
            NBlue::callback->patchset.HWPatch3->arrsize = arrsize(kHSWHWProbeHS3Original);
            NBlue::callback->patchset.HWPatch4->find = kHSWHWProbeHS4Original;
            NBlue::callback->patchset.HWPatch4->repl = kHSWHWProbeHS4Patched;
            NBlue::callback->patchset.HWPatch4->arrsize = arrsize(kHSWHWProbeHS4Original);

            DBGLOG("hsw", "Setting patches for MTLDriver...");

            NBlue::callback->patchset.MTLPatch1 = &kBinModInfoHSWMTLHS1;
            NBlue::callback->patchset.MTLPatch2 = &kBinModInfoHSWMTLHS2;

            DBGLOG("hsw", "Setting patches for VADriver...");

            NBlue::callback->patchset.VAPatch1 = &kBinModInfoHSWVAHS1;
            NBlue::callback->patchset.VAPatch2 = &kBinModInfoHSWVAHS2;
            NBlue::callback->patchset.VAPatch3 = &kBinModInfoHSWVAHS3;
            NBlue::callback->patchset.VAPatch4 = &kBinModInfoHSWVAHS4;
            NBlue::callback->patchset.VAPatch5 = &kBinModInfoHSWVAHS5;

        } else if (NBlue::callback->kVer == 18) {
            DBGLOG("hsw", "Mojave patchset unavailable! system breakage may occur!");
        } else if (NBlue::callback->kVer == 19) {
            DBGLOG("hsw", "Catalina patchset unavailable! system breakage may occur!");
        } else if (NBlue::callback->kVer == 20) {
            DBGLOG("hsw", "Big Sur patchset unavailable! system breakage may occur!");
        };
        DBGLOG("hsw", "Configured.");
        return true;
    }

    return false;
}

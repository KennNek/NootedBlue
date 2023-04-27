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

void HSW::init() {
    callback = this;
    lilu.onKextLoadForce(&kextHSWFB);
    lilu.onKextLoadForce(&kextHSWHW);
}

bool HSW::processDrivers(KernelPatcher &patcher, size_t index) {
    if (kextHSWHW.loadIndex == index) {
		if (IGT1F::callback->kVer == 16) {
			KernelPatcher::LookupPatch patches[] = {
				{&kextHSWHW, kHSWHWProbeSierra1Original, kHSWHWProbeSierra1Patched, arrsize(kHSWHWProbeSierra1Original), 1},
				{&kextHSWHW, kHSWHWProbeSierra2Original, kHSWHWProbeSierra2Patched, arrsize(kHSWHWProbeSierra2Original), 1},
				{&kextHSWHW, kHSWHWProbeSierra3Original, kHSWHWProbeSierra3Patched, arrsize(kHSWHWProbeSierra3Original), 1},
				{&kextHSWHW, kHSWHWProbeSierra4Original, kHSWHWProbeSierra4Patched, arrsize(kHSWHWProbeSierra4Original), 1},
			};
			for (auto &patch : patches) {
				patcher.applyLookupPatch(&patch);
				patcher.clearError();
			}
		} else if (IGT1F::callback->kVer == 17) {
			KernelPatcher::LookupPatch patches[] = {
				{&kextHSWHW, kHSWProbeHS1Original, kHSWProbeHS1Patched, arrsize(kHSWProbeHS1Original), 1},
				{&kextHSWHW, kHSWProbeHS2Original, kHSWProbeHS2Patched, arrsize(kHSWProbeHS2Original), 1},
				{&kextHSWHW, kHSWProbeHS3Original, kHSWProbeHS3Patched, arrsize(kHSWProbeHS3Original), 1},
				{&kextHSWHW, kHSWProbeHS4Original, kHSWProbeHS4Patched, arrsize(kHSWProbeHS4Original), 1},
 			};
			for (auto &patch : patches) {
				patcher.applyLookupPatch(&patch);
				patcher.clearError();
			}
		} else if (IGT1F::callback->kVer == 18) {
			KernelPatcher::LookupPatch patches[] = {
				{&kextHSWHW, kHSWHWMojave1Original, kHSWHWMojave1Patched, arrsize(kHSWHWMojave1Original), 1},
				{&kextHSWHW, kHSWHWMojave2Original, kHSWHWMojave2Patched, arrsize(kHSWHWMojave2Original), 1},
				{&kextHSWHW, kHSWHWMojave3Original, kHSWHWMojave3Patched, arrsize(kHSWHWMojave3Original), 1},
				{&kextHSWHW, kHSWHWMojave4Original, kHSWHWMojave4Patched, arrsize(kHSWHWMojave4Original), 1},
			};
			for (auto &patch : patches) {
				patcher.applyLookupPatch(&patch);
				patcher.clearError();
			}
		} else if (IGT1F::callback->kVer == 19) {
			KernelPatcher::LookupPatch patches[] = {
				{&kextHSWHW, kHSWProbeCatalina1Original, kHSWProbeCatalina1Patched, arrsize(kHSWProbeCatalina1Original), 1},
				{&kextHSWHW, kHSWProbeCatalina2Original, kHSWProbeCatalina2Patched, arrsize(kHSWProbeCatalina2Original), 1},
				//{&kextHSWHW, kHSWProbeCatalina3Original, kHSWProbeCatalina3Patched, arrsize(kHSWProbeCatalina3Original), 1},
				{&kextHSWHW, kHSWProbeCatalina4Original, kHSWProbeCatalina4Patched, arrsize(kHSWProbeCatalina4Original), 1},
			};
			for (auto &patch : patches) {
				patcher.applyLookupPatch(&patch);
				patcher.clearError();
			}
		} else if (IGT1F::callback->kVer == 20) {
			DBGLOG("hsw", "Big Sur patches aren't in the work yet");
		};
        DBGLOG("hsw", "Applied patches for HD5kGraphics");
        return true;
    }

    return false;
}

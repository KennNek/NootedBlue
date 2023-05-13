//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_nblue.hpp"
#include "kern_gen8.hpp"
#include "kern_gen9.hpp"
#include "kern_gen9_5.hpp"
#include "kern_hsw.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <Headers/kern_util.hpp>

NBlue *NBlue::callback = nullptr;

static Gen8 gen8;
static Gen9 gen9;
static Gen9_5 gen9_5;
static HSW hsw;

void NBlue::init() {
    callback = this;
    lilu.onPatcherLoadForce(
        [](void *user, KernelPatcher &patcher) { static_cast<NBlue *>(user)->processPatcher(patcher); }, this);
    lilu.onKextLoadForce(
        nullptr, 0,
        [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
            static_cast<NBlue *>(user)->processKext(patcher, index, address, size);
        },
        this);
    gen8.init();
    gen9.init();
    hsw.init();
}

void NBlue::processPatcher(KernelPatcher &patcher) {
    auto *devInfo = DeviceInfo::create();
    if (devInfo) {
        devInfo->processSwitchOff();

        this->iGPU = OSDynamicCast(IOPCIDevice, devInfo->videoBuiltin);
        PANIC_COND(!this->iGPU, "nblue", "videoBuiltin is not IOPCIDevice");

        WIOKit::renameDevice(this->iGPU, "IGPU");
        WIOKit::awaitPublishing(this->iGPU);
        char name[256] = {0};
        for (size_t i = 0, ii = 0; i < devInfo->videoExternal.size(); i++) {
            auto *device = OSDynamicCast(IOPCIDevice, devInfo->videoExternal[i].video);
            if (device) {
                snprintf(name, arrsize(name), "GFX%zu", ii++);
                WIOKit::renameDevice(device, name);
                WIOKit::awaitPublishing(device);
            }
        }

        static uint8_t builtin[] = {0x01};
        this->kVer = getKernelVersion();
        this->iGPU->setProperty("built-in", builtin, arrsize(builtin));
        this->deviceId = WIOKit::readPCIConfigValue(this->iGPU, WIOKit::kIOPCIConfigDeviceID);

        DeviceInfo::deleter(devInfo);
    } else {
        SYSLOG("igt1f", "Failed to create DeviceInfo");
    }

    if (this->kVer == 20) {
        KernelPatcher::RouteRequest requests[] = {
            {"_cs_validate_page", csValidatePage, this->orgCsValidatePage},
        };

        size_t num = arrsize(requests);

        PANIC_COND(!patcher.routeMultipleLong(KernelPatcher::KernelID, requests, num), "igt1f",
            "Failed to route kernel symbols");
    }
}

bool NBlue::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    if (hsw.configurePatches(index)) {
        DBGLOG("igt1f", "Applied Haswell configuration");
    } else if (gen8.processKext(patcher, index, address, size)) {
        DBGLOG("igt1f", "Applied Generation 8 configuration");
    } else if (gen9.processKext(patcher, index, address, size)) {
        DBGLOG("igt1f", "Applied Skylake/Apollo Lake configuration");
    }

    KernelPatcher::LookupPatch patches[] = {
        {this->patchset.HWKext, this->patchset.HWPatch1->find, this->patchset.HWPatch1->repl,
            this->patchset.HWPatch1->arrsize, 1},
        {this->patchset.HWKext, this->patchset.HWPatch2->find, this->patchset.HWPatch2->repl,
            this->patchset.HWPatch2->arrsize, 1},
        {this->patchset.HWKext, this->patchset.HWPatch3->find, this->patchset.HWPatch3->repl,
            this->patchset.HWPatch3->arrsize, 1},
        {this->patchset.HWKext, this->patchset.HWPatch4->find, this->patchset.HWPatch4->repl,
            this->patchset.HWPatch4->arrsize, 1},
    };
    for (auto &patch : patches) {
        patcher.applyLookupPatch(&patch);
        SYSLOG_COND(patcher.getError() != KernelPatcher::Error::NoError, "igt1f", "Failed to apply %c patch: %d",
            this->patchset.MiscNames->hw, patcher.getError());
        patcher.clearError();
    }

    DBGLOG("igt1f", "Applying patches for %c for OS version %c", callback->patchset.MiscNames->mtl,
        this->patchset.MiscNames->os);

    lilu.onProcLoadForce(this->patchset.MTLProcInfo, 1, nullptr, nullptr, this->patchset.MTLPatch1, 1);
    lilu.onProcLoadForce(this->patchset.MTLProcInfo, 1, nullptr, nullptr, this->patchset.MTLPatch2, 1);
    if (this->kVer > 16 && this->igfxGen == iGFXGen::Haswell) {
        // 10.13 and 10.14 have 3 MTLDriver patches compared to 2 for Sierra, likely due to Metal 2 or something
        lilu.onProcLoad(this->patchset.MTLProcInfo, 1, nullptr, nullptr, this->patchset.MTLPatch3, 1);
    }

    // argument here for debugging since VA isn't critical for operations
    if (!checkKernelArgument("-patchbundlesoff")) {
        DBGLOG("igt1f", "Applying patches for %c", this->patchset.MiscNames->va);
        lilu.onProcLoad(this->patchset.VAProcInfo, 1, nullptr, nullptr, this->patchset.VAPatch1, 1);
        lilu.onProcLoad(this->patchset.VAProcInfo, 1, nullptr, nullptr, this->patchset.VAPatch2, 1);
        lilu.onProcLoad(this->patchset.VAProcInfo, 1, nullptr, nullptr, this->patchset.VAPatch3, 1);
        lilu.onProcLoad(this->patchset.VAProcInfo, 1, nullptr, nullptr, this->patchset.VAPatch4, 1);
        lilu.onProcLoad(this->patchset.VAProcInfo, 1, nullptr, nullptr, this->patchset.VAPatch5, 1);
    }

    return true;
}

void NBlue::csValidatePage(vnode *vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data,
    int *validated_p, int *tainted_p, int *nx_p) {
    FunctionCast(csValidatePage, callback->orgCsValidatePage)(vp, pager, page_offset, data, validated_p, tainted_p,
        nx_p);

    char path[PATH_MAX];
    int pathlen = PATH_MAX;
    if (LIKELY(!vn_getpath(vp, path, &pathlen))) {
        if (UNLIKELY(UserPatcher::matchSharedCachePath(path))) {
            if (callback->igfxGen == iGFXGen::Haswell) {
                if (callback->kVer == 20) {
                    DBGLOG("igt1f", "Placeholder code for the future, usually this is when we would start patching "
                                    "bundles from the dyld cache");
                }
            }
        }
    }
}

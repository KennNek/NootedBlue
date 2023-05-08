//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_igt1f_hpp
#define kern_igt1f_hpp
#include "kern_bdw.hpp"
#include "kern_hsw.hpp"
#include "kern_skl.hpp"
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_user.hpp>
#include <Headers/kern_util.hpp>
#include <IOKit/pci/IOPCIDevice.h>

enum struct iGFXGen {
    Haswell,    // Haswell had no lower end rebrands, so it gets it's name plastered here
    Gen8,
    Gen9,
    Gen95,    // Gen 9.5, aka: KBL, CFL, GML, CML, CNL, etc
    Gen11,    // Ice Lake kexts support G1 with device-id spoofs, this, however, is inconvenient, TBWO
    Unknown,
};

struct KernelPatch {
    const uint8_t *find;
    const uint8_t *repl;
    size_t arrsize;
};

struct MiscNames {
    const char *hw;
    const char *fb;
    const char *va;
    const char *mtl;
    const char *os;
};

// Allows for easy flexibility across multiple generations of architectures, it also makes patching more efficent and
// maleable across OS versions
struct Patchset {
    MiscNames *MiscNames;
    KernelPatcher::KextInfo *HWKext;
    KernelPatcher::KextInfo *FBKext;
    KernelPatch *HWPatch1;
    KernelPatch *HWPatch2;
    KernelPatch *HWPatch3;
    KernelPatch *HWPatch4;
    KernelPatch *HWPatch5;
    UserPatcher::BinaryModInfo *MTLPatch1;
    UserPatcher::BinaryModInfo *MTLPatch2;
    UserPatcher::BinaryModInfo
        *MTLPatch3;    // unused by HSW's Sierra patches, so we implement a check in processDrivers
    UserPatcher::BinaryModInfo *VAPatch1;
    UserPatcher::BinaryModInfo *VAPatch2;
    UserPatcher::BinaryModInfo *VAPatch3;
    UserPatcher::BinaryModInfo *VAPatch4;
    UserPatcher::BinaryModInfo *VAPatch5;
    UserPatcher::ProcInfo *MTLProcInfo;
    UserPatcher::ProcInfo *VAProcInfo;
};

class IGT1F {
    friend class BDW;
    friend class HSW;
    friend class SKL;

    public:
    static IGT1F *callback;
    void init();
    void processPatcher(KernelPatcher &patcher);
    bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

    Patchset patchset;

    private:
    int kVer {0};
    iGFXGen igfxGen = iGFXGen::Unknown;
    uint32_t deviceId {0};
    uint16_t revision {0};
    IOPCIDevice *iGPU {nullptr};
    mach_vm_address_t orgCsValidatePage {0};
    void processDrivers(UserPatcher::ProcInfo &mtlprocinfo, UserPatcher::ProcInfo &vaprocinfo);

    static OSMetaClassBase *wrapSafeMetaCast(const OSMetaClassBase *anObject, const OSMetaClass *toMeta);
    static void csValidatePage(vnode *vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data,
        int *validated_p, int *tainted_p, int *nx_p);
};

#endif /* kern_igt1f_hpp */

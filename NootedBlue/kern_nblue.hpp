//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_igt1f_hpp
#define kern_igt1f_hpp
#include "kern_gen8.hpp"
#include "kern_gen9.hpp"
#include "kern_hsw.hpp"
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_user.hpp>
#include <Headers/kern_util.hpp>
#include <IOKit/pci/IOPCIDevice.h>

enum struct iGFXGen {
    Haswell,    // Haswell had no lower end rebrands, so it gets it's name plastered here
    Gen8,       // Broadwell & Braswell/Cherryview
    Gen9,       // Skylake & Apollo Lake
    Gen9_5,     // Coffee Lake, Comet Lake, Gemini Lake, Kaby Lake
    Gen11,      // Ice Lake kexts support G1 with device-id spoofs, this, however, is inconvenient, also Elkhart/Jasper
                // Lake exist, pain.
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
    UserPatcher::BinaryModInfo *MTLPatch3;
    UserPatcher::BinaryModInfo *VAPatch1;
    UserPatcher::BinaryModInfo *VAPatch2;
    UserPatcher::BinaryModInfo *VAPatch3;
    UserPatcher::BinaryModInfo *VAPatch4;
    UserPatcher::BinaryModInfo *VAPatch5;
    UserPatcher::ProcInfo *MTLProcInfo;
    UserPatcher::ProcInfo *VAProcInfo;
};

class NBlue {
    friend class Gen8;
    friend class Gen9;
    friend class Gen9_5;
    friend class Gen11;
    friend class HSW;

    public:
    static NBlue *callback;
    void init();
    void processPatcher(KernelPatcher &patcher);
    bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

    Patchset patchset;

    private:
    bool isCflDerivative = false;
    bool disableAllPatches = false;
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

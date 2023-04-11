//  Copyright © 2022-2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_igt1f_hpp
#define kern_igt1f_hpp
#include "kern_hsw.hpp"
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_util.hpp>
#include <IOKit/pci/IOPCIDevice.h>

enum struct iGFXGen {
    Haswell,
    Braswell,
    Skylake,
    ApolloLake,
    // KabyLake, // In the future, the newer architectures may be worked on
    // CoffeeLake,
    // CometLake,
    Unknown,
};

class IGT1F {
    friend class BSW;
    friend class HSW;

    public:
    static IGT1F *callback;
    void init();
    void processPatcher(KernelPatcher &patcher);
    void processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

    private:
    iGFXGen igfxGen = iGFXGen::Unknown;
    uint32_t deviceId {0};
    uint16_t revision {0};
    IOPCIDevice *iGPU {nullptr};
};

#endif /* kern_igt1f_hpp */

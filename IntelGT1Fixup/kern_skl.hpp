//  Copyright © 2022-2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_skl_hpp
#define kern_skl_hpp
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_util.hpp>

class SKL {
    public:
    static SKL *callback;
    void init();
    bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
};

#endif /* kern_hsw_hpp */

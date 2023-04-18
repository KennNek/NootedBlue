//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_hsw_hpp
#define kern_hsw_hpp
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_util.hpp>

class HSW {
    public:
    static HSW *callback;
    void init();
    bool processDrivers(KernelPatcher &patcher, size_t index);
};

#endif /* kern_hsw_hpp */

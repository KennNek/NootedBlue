//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_bdw_patches_hpp
#define kern_bdw_patches_hpp
#include <Headers/kern_user.hpp>
#include <Headers/kern_util.hpp>
/**
 * `Broadwell Bundle paths`
 */

static const char kBDWMTLDriverPath[] = "/System/Library/Extensions/AppleIntelBDWGraphicsMTLDriver.bundle/Contents/"
                                        "MacOS/AppleIntelBDWGraphicsMTLDriver";
static const char kBDWVADriverPath[] = "/System/Library/Extensions/AppleIntelBDWGraphicsVADriver.bundle/Contents/"
                                       "MacOS/AppleIntelBDWGraphicsVADriver";
/**
 * `Braswell & Broadwell Patch Set for Mojave`
 *  In active development
 */

/** Based off of WhateverGreen code*/

enum : uint32_t {
    BDWMTL = 1,
    BDWVA = 2,
    BDWMTLMojave = 3,
    BDWVAMojave = 4,
    BDWMTLCatalina = 5,
    BDWVACatalina = 6,
};

static uint32_t kBDWMTLLength = sizeof("/System/Library/Extensions/AppleIntelBDWGraphicsMTLDriver.bundle/Contents/"
                                       "MacOS/AppleIntelBDWGraphicsMTLDriver") -
                                1;
static uint32_t kBDWVALength = sizeof("/System/Library/Extensions/AppleIntelBDWGraphicsVADriver.bundle/Contents/MacOS/"
                                      "AppleIntelBDWGraphicsVADriver") -
                               1;

static UserPatcher::ProcInfo kProcInfoBDWMTL {kBDWMTLDriverPath, kBDWMTLLength, BDWMTL};
static UserPatcher::ProcInfo kProcInfoBDWVA {kBDWVADriverPath, kBDWVALength, BDWVA};

#endif /* kern_bdw_patches_hpp */

//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_gen8_patches_hpp
#define kern_gen8_patches_hpp
#include <Headers/kern_user.hpp>
#include <Headers/kern_util.hpp>
/**
 * `Broadwell Bundle paths`
 */

static const char kGen8MTLDriverPath[] = "/System/Library/Extensions/AppleIntelBDWGraphicsMTLDriver.bundle/Contents/"
                                        "MacOS/AppleIntelBDWGraphicsMTLDriver";
static const char kGen8VADriverPath[] = "/System/Library/Extensions/AppleIntelBDWGraphicsVADriver.bundle/Contents/"
                                       "MacOS/AppleIntelBDWGraphicsVADriver";
/**
 * `Braswell & Broadwell Patch Set for Mojave`
 *  In active development
 */

/** Based off of WhateverGreen code*/

enum : uint32_t {
    Gen8MTL = 1,
    Gen8VA = 2,
    Gen8MTLMojave = 3,
    Gen8VAMojave = 4,
    Gen8MTLCatalina = 5,
    Gen8VACatalina = 6,
};

static uint32_t kGen8MTLLength = sizeof("/System/Library/Extensions/AppleIntelGen8GraphicsMTLDriver.bundle/Contents/"
                                       "MacOS/AppleIntelBDWGraphicsMTLDriver") -
                                1;
static uint32_t kGen8VALength = sizeof("/System/Library/Extensions/AppleIntelGen8GraphicsVADriver.bundle/Contents/MacOS/"
                                      "AppleIntelBDWGraphicsVADriver") -
                               1;

static UserPatcher::ProcInfo kProcInfoGen8MTL {kGen8MTLDriverPath, kGen8MTLLength, Gen8MTL};
static UserPatcher::ProcInfo kProcInfoGen8VA {kGen8VADriverPath, kGen8VALength, Gen8VA};

#endif /* kern_gen8_patches_hpp */

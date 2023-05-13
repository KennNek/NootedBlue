//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/plugin_start.hpp>

static NBlue nblue;

static const char *bootargOff[] = {
    "-nblueoff",
};

static const char *bootargDebug[] = {
    "-nbluedbg",
};

static const char *bootargBeta[] = {
    "-nbluebeta",
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal | LiluAPI::AllowSafeMode,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Monterey,
    KernelVersion::HighSierra,
    []() { nblue.init(); },
};

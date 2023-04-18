//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_igt1f.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/plugin_start.hpp>

static IGT1F igt1f;

static const char *bootargOff[] = {
    "-igt1foff",
};

static const char *bootargDebug[] = {
    "-igt1fdbg",
};

static const char *bootargBeta[] = {
    "-igt1fbeta",
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
    []() { igt1f.init(); },
};

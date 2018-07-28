#pragma once

// Platform Detection
#include <remod/platform_detection.h>

// Global bridge system
#include <remod/module.h>
#include <remod/global_pointer.h>
#include <remod/global_data_pointer.h>
#include <remod/resolve_strategy.h>
#include <remod/remod_functions.h>

// Definitions
#include <remod/remod_definitions.h>

// Win32-specific files
#ifdef REMOD_WIN_DETECTED
#include <remod/remod_definitions_win32.h>
#endif



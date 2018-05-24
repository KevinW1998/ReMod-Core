#pragma once

// Platform Detection
#include <remod/platform_detection.h>

#include <remod/module.h>
#include <remod/global_pointer.h>
#include <remod/global_data_pointer.h>
#include <remod/resolve_strategy.h>
#include <remod/remod_functions.h>

// Win32-specific files
#ifdef REMOD_WIN_DETECTED
#include <remod/remod_win32.h>
#endif



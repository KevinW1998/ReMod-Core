#pragma once

#include "platform_detection.h"

#ifdef REMOD_WIN_DETECTED
#include "win32/patch_engine_win32.h"
namespace remod
{
	using patch_engine = win32::patch_engine_win32;
}
#else
#error "Unsupported platform for the patch engine"
#endif



#pragma once

// OS detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define REMOD_WIN_DETECTED
#else

#endif

// bitness detection, thanks to https://stackoverflow.com/a/1505839/5082374
#include <cstdint>
#if INTPTR_MAX == INT32_MAX
	#define REMOD_IS_32_BIT
#elif INTPTR_MAX == INT64_MAX
	#define REMOD_IS_64_BIT
#else
	#error "Environment not 32 or 64-bit."
#endif


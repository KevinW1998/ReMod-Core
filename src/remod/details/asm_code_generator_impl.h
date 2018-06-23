#pragma once

#include <remod/details/asm_code_generator.h>
#include <remod/platform_detection.h>

#ifdef REMOD_IS_32_BIT
#include "asm_code_generator_x86.h"
#else
#include "asm_code_generator_x64.h"
#endif



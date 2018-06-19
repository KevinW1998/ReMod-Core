#pragma once

#include "../platform_detection.h"

namespace remod::details
{
	class asm_code_generator_x86;
	class asm_code_generator_x64;

#ifdef REMOD_IS_32_BIT
	using asm_code_generator = asm_code_generator_x86;
#else
	using asm_code_generator = asm_code_generator_x64;
#endif
}


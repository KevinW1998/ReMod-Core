#pragma once
#include "base_asm_code_generator.h"
#include "remod/calling_convention.h"
#include "remod/detour_point.h"

namespace remod::details
{
	class asm_code_generator_x86 : base_asm_code_generator
	{
	public:

		// Stable API:
		void* generator_call_conv_detour(detour_point to_convert, void* context_value, calling_convention target_call_conv, void* func_to_call);
	};
}

#pragma once

#include <asmjit/asmjit.h>
#include "remod/calling_convention.h"
#include <vector>
#include <tuple>

namespace remod::details
{
	class base_asm_code_generator
	{
		asmjit::JitRuntime m_runtime;
	protected:
		asmjit::JitRuntime& get_runtime();

		// Helper functions:
		int calculate_stack_to_manage(calling_convention conv, const std::vector<size_t>& args);
	public:

	};
}

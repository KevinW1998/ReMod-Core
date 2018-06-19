#pragma once

#include <asmjit/asmjit.h>

namespace remod::details
{
	class base_asm_code_generator
	{
		asmjit::JitRuntime m_runtime;
	protected:
		asmjit::JitRuntime& get_runtime()
		{
			return m_runtime;
		}
	public:

	};
}

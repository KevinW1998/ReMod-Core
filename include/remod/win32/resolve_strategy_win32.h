#pragma once

#include <cstdint>

namespace remod {
	class module;
}

namespace remod::win32
{
	class resolve_strategy_module_handle
	{
	public:
		static bool resolve(std::uintptr_t& to_resolve, const module& module);
	};
}


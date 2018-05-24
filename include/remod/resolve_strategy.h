#pragma once

#include <cstdint>

namespace remod
{
	class module;

	class resolve_strategy_noop
	{
	public:
		static bool resolve(std::uintptr_t& to_resolve, const module& module);
	};

	class resolve_strategy_static
	{
	public:
		static bool resolve(std::uintptr_t& to_resolve, const module& module);
	};

}


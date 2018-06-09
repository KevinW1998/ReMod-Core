#pragma once

#include "common.h"
#include <cstdint>


namespace remod
{
	class module;

	class REMOD_CORE_EXPORT resolve_strategy_noop
	{
	public:
		static bool resolve(std::uintptr_t& to_resolve, const module& module);
	};

}


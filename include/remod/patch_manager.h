#pragma once

#include "common.h"
#include "base_patch_manager.h"
#include "module.h"

namespace remod
{
	template<typename ResolveStrategy>
	class REMOD_CORE_EXPORT patch_manager : base_patch_manager
	{
		module m_mod;
	public:
		patch_manager(const module& mod) : m_mod(mod)
		{}




	};
}





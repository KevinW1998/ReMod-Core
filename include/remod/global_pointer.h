#pragma once

#include "details/global_pointer_base.h"
#include "module.h"

namespace remod
{
	template<typename ResolveStrategy>
	class global_pointer : public details::global_pointer_base
	{
		module m_module;
	public:
		global_pointer(std::uintptr_t ptr, const module& module = main_module) : 
			global_pointer_base(ptr), 
			m_module(module)
		{}
		~global_pointer() = default;

	protected:
		virtual bool resolve_pointer() override;
	};

	template <typename ResolveStrategy>
	bool global_pointer<ResolveStrategy>::resolve_pointer()
	{
		return ResolveStrategy::resolve(get_pointer_raw_ref(), m_module);
	}
}


#pragma once

#include "details/global_pointer_base.h"
#include "module.h"

namespace remod
{
	template<typename ResolveStrategy, const module& Module = main_module>
	class global_pointer : public details::global_pointer_base
	{
	public:
		using details::global_pointer_base::global_pointer_base;
		~global_pointer() = default;

	protected:
		virtual bool resolve_pointer() override;
	};


	template <typename ResolveStrategy, const module& Module>
	bool global_pointer<ResolveStrategy, Module>::resolve_pointer()
	{
		return ResolveStrategy::resolve(get_pointer_raw_ref(), Module);
	}
}


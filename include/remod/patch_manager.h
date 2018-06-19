#pragma once

#include "common.h"
#include "base_patch_manager.h"
#include "module.h"
#include "trackable_wrapper.h"
#include "trackable_function_patch.h"
#include "detour_point.h"
#include "function_signature.h"

namespace remod
{
	template<typename ResolveStrategy>
	class REMOD_CORE_EXPORT patch_manager : base_patch_manager
	{
		module m_mod;
	public:
		patch_manager(const module& mod) : m_mod(mod)
		{}

		template<typename Ret, typename... Args>
		trackable_wrapper<trackable_function_patch<Ret(Args...)>> apply(const detour_point& detour_point_to_apply, function_signature<Ret(Args...)>)
		{
			static_assert(sizeof(Ret) <= 4, "Currently only return values less than or equal 4 bytes are supported!");

			// Resolve address
			std::uintptr_t addr = detour_point_to_apply.get_offset();
			ResolveStrategy::resolve(addr, m_mod);

			// TODO: Create trackable_function_patch
			trackable_function_patch<Ret(Args...)>* func_patch = new trackable_function_patch<Ret(Args...)>();
			get_patch_store()->add_patch(func_patch);
			func_patch->set_original_function_call(detour_point_to_apply.get_original_function_call());
			func_patch->set_return_value_source(detour_point_to_apply.get_return_value_source());

			// TODO: Validate signature with added arguments

			// TODO: Add call_proxy function --> calling convetions
			auto proxy = +[](trackable_function_patch<Ret(Args...)>* patch, Args... args)
			{
				return patch(args...);
			};

			// TODO: Generate asm stub
			generate_function_patch(addr, func_patch, proxy, detour_point_to_apply);

			return { get_patch_store(), func_patch };
		}


	};
}





#pragma once

#include "common.h"
#include "base_patch_manager.h"
#include "module.h"
#include "trackable_wrapper.h"
#include "trackable_function_patch.h"
#include "detour_point.h"
#include "function_signature.h"
#include "details/function_traits.h"

namespace remod
{
	template<typename ResolveStrategy>
	class patch_manager : base_patch_manager
	{
		module m_mod;

		template<typename Ret, typename... Args>
		trackable_wrapper<trackable_function_patch<Ret(Args...)>> apply_impl(const detour_point& detour_point_to_apply, details::function_traits<Ret(Args...)>)
		{
			
			static_assert(sizeof(Ret) <= 4, "Currently only return values less than or equal 4 bytes are supported!");

			// Resolve address
			std::uintptr_t addr = detour_point_to_apply.get_offset();
			ResolveStrategy::resolve(addr, m_mod);

			// std::function<Ret(Args...)> test_func;

			// TODO: Create trackable_function_patch
			trackable_function_patch<Ret(Args...)>* func_patch = new trackable_function_patch<Ret(Args...)>;
			
			get_patch_store()->add_patch(func_patch);
			func_patch->set_original_function_call(detour_point_to_apply.get_original_function_call());
			func_patch->set_return_value_source(detour_point_to_apply.get_return_value_source());
			func_patch->set_original_function(reinterpret_cast<void*>(addr));
			
			auto proxy = +[](trackable_function_patch<Ret(Args...)>* patch, Args... args)
			{
				return (*patch)(args...);
			};
			
			
			// Generate the patch and apply the patch
			generate_function_patch(addr, func_patch, reinterpret_cast<void*>(proxy), detour_point_to_apply);
			func_patch->patch();
			
			return { get_patch_store(), func_patch };
		}
	public:
		patch_manager(const module& mod = remod::main_module) : m_mod(mod)
		{}

		template<typename Func>
		auto apply(const detour_point& detour_point_to_apply, Func)
		{
			return apply_impl(detour_point_to_apply, details::function_traits<Func>{});
		}


	};
}





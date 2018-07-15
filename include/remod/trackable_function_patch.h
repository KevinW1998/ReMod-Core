#pragma once

#include <functional>

#include "trackable_patch.h"
#include "return_value_source.h"
#include "original_function_call.h"
#include "details/calling_convention_type_helper.h"
#include "details/trackable_function_patch_base.h"

namespace remod
{
	template<typename>
	class trackable_function_patch;
	
	template<typename Ret, typename... Args>
	class trackable_function_patch<Ret(Args...)> : public trackable_function_patch_base {
		std::vector<std::function<Ret(Args...)>> m_detour_functions;
	public:
		// Inherit ctor
		using trackable_function_patch_base::trackable_function_patch_base;

		template<typename Func>
		void add_detour_function(Func&& func)
		{
			m_detour_functions.push_back(std::forward<Func>(func));
		}

		// TODO: Extract to own function?
		Ret operator()(Args... args) const {
			Ret rValOrig{};
			if (m_orig_func_call == original_function_call::before_detours && m_orig_func)
				rValOrig = details::call_by_convetion<Ret, Args...>(m_call_conv, m_orig_func, args...);

			Ret rValDetour{};
			if (m_detour_functions.size() > 0u)
			{
				rValDetour = m_detour_functions[0](args...);
				for (int i = 1; i < m_detour_functions.size(); i++)
				{
					m_detour_functions[i](args...);
				}
			}
			
			if (m_orig_func_call == original_function_call::after_detours && m_orig_func)
				rValOrig = details::call_by_convetion<Ret, Args...>(m_call_conv, m_orig_func, args...);

			if (m_ret_val_source == return_value_source::original_function)
				return rValOrig;
			return rValDetour;
		}
	};
}



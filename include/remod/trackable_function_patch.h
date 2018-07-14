#pragma once

#include <functional>

#include "trackable_patch.h"
#include "return_value_source.h"
#include "original_function_call.h"
#include "details/calling_convention_type_helper.h"

namespace remod
{
	template<typename>
	class trackable_function_patch;
	
	template<typename Ret, typename... Args>
	class trackable_function_patch<Ret(Args...)> : public trackable_patch {
		std::vector<std::function<Ret(Args...)>> m_detour_functions;
		return_value_source m_ret_val_source = return_value_source::first_detour_function;
		original_function_call m_orig_func_call = original_function_call::after_detours;
		calling_convention m_call_conv = calling_convention::conv_default;
		void* m_orig_func;
	public:
		// Inherit ctor
		using trackable_patch::trackable_patch;

		void add_detour_function(const std::function<Ret(Args...)>& func)
		{
			m_detour_functions.push_back(func);
		}

		void add_detour_function(std::function<Ret(Args...)>&& func)
		{
			m_detour_functions.push_back(std::move(func));
		}

		// TODO: Extract to own function?
		Ret operator()(Args... args) const {
			Ret rValOrig;
			if (m_orig_func_call == original_function_call::before_detours && m_orig_func)
				rValOrig = details::call_by_convetion<Ret, Args...>(m_call_conv, m_orig_func, args...);

			Ret rValDetour;
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

		void set_return_value_source(return_value_source source)
		{
			m_ret_val_source = source;
		}

		void set_original_function_call(original_function_call orig_call)
		{
			m_orig_func_call = orig_call;
		}

		void set_original_function(void* orig)
		{
			m_orig_func = orig;
		}
	};
}



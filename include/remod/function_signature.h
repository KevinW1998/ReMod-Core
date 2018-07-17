#pragma once

#include "details/function_traits.h"

namespace remod
{
	template<typename>
	struct function_signature;
	
	template<typename Ret, typename... Args>
	struct function_signature<Ret(Args...)> {};

	namespace details
	{
		template<typename Ret, typename... Args>
		auto signature_from_function_impl(function_traits<Ret(Args...)>)
		{
			return function_signature<Ret(Args...)>{};
		}
	}

	template<typename Func>
	auto signature_from_function(Func)
	{
		return details::signature_from_function_impl(details::function_traits<Func>{});
	}
}



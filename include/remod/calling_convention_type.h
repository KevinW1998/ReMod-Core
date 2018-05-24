#pragma once

#include <remod/details/calling_convention_type_helper.h>


namespace remod
{
	template<calling_convention Conv, typename Ret, typename... Args>
	using calling_convention_type = typename details::convention_type<Conv, Ret, Args...>::function_type;
}


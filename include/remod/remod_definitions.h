#pragma once

#include <remod/global_pointer.h>
#include <remod/global_data_pointer.h>
#include <remod/global_function_pointer.h>
#include <remod/resolve_strategy.h>
#include <remod/global_data.h>
#include "global_string.h"

namespace remod
{
	using global_pointer_no_resolve = global_pointer<resolve_strategy_noop>;

	template<typename T>
	using global_data_pointer_no_resolve = global_data_pointer<T, resolve_strategy_noop>;

	template<typename T>
	using global_data_no_resolve = global_data<T, resolve_strategy_noop>;

	template<calling_convention Conv, typename T>
	using global_function_pointer_no_resolve = global_function_pointer<Conv, resolve_strategy_noop, T>;

	using global_string_no_resolve = global_string<resolve_strategy_noop>;

	using global_wstring_no_resolve = global_wstring<resolve_strategy_noop>;
}

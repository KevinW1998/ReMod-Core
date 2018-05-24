#pragma once

#include <remod/global_pointer.h>
#include <remod/global_data_pointer.h>

#include <remod/win32/resolve_strategy_win32.h>

namespace remod
{
	template<const module& Module = main_module>
	using global_pointer_win32 = global_pointer<win32::resolve_strategy_module_handle, Module>;

	template<typename T, const module& Module = main_module>
	using global_data_pointer_win32 = global_data_pointer<T, win32::resolve_strategy_module_handle, Module>;
}



#pragma once

#include <remod/global_pointer.h>
#include <remod/resolve_strategy.h>
#include "global_data_pointer.h"

namespace remod
{
	using global_pointer_no_resolve = global_pointer<resolve_strategy_noop>;

	using global_pointer_static_resolve = global_pointer<resolve_strategy_static>;

	template<typename T>
	using global_data_pointer_no_resolve = global_data_pointer<T, resolve_strategy_noop>;

	template<typename T>
	using global_data_pointer_static_resolve = global_data_pointer<T, resolve_strategy_static>;
}

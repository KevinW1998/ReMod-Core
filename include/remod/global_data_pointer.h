#pragma once

#include <remod/global_pointer.h>
#include <cassert>

namespace remod
{
	template<typename T, typename ResolveStrategy, const module& Module = main_module>
	class global_data_pointer : public global_pointer<ResolveStrategy, Module>
	{
	public:
		using global_pointer<ResolveStrategy, Module>::global_pointer;

		T* operator->()
		{
			assert(is_resolved());

			return *reinterpret_cast<T**>(get_pointer());
		}

		T& operator* ()
		{
			assert(is_resolved());

			return **reinterpret_cast<T**>(get_pointer());
		}

		T* Get()
		{
			return *reinterpret_cast<T**>(GetPointer());
		}
	};
}


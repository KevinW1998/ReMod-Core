#pragma once

#include <remod/global_pointer.h>
#include <cassert>

namespace remod
{
	template<typename T, typename ResolveStrategy>
	class global_data_pointer : public global_pointer<ResolveStrategy>
	{
	public:
		using global_pointer<ResolveStrategy>::global_pointer;

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


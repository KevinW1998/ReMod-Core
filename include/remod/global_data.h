#pragma once

#include <remod/global_pointer.h>

namespace remod
{
	template<typename T, typename ResolveStrategy>
	class global_data : public global_pointer<ResolveStrategy>
	{
	public:
		using global_pointer<ResolveStrategy>::global_pointer;

		operator T&() {
			return *reinterpret_cast<T*>(get_pointer());
		}

		operator const T&() const {
			return *reinterpret_cast<T*>(get_pointer());
		}

		T& get_ref()
		{
			return *reinterpret_cast<T*>(get_pointer());
		}
	};
}


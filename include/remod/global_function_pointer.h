#pragma once

#include <remod/global_pointer.h>
#include <remod/calling_convention.h>
#include <remod/calling_convention_type.h>

namespace remod
{
	template<calling_convention Conv, typename ResolveStrategy, typename T>
	class global_function_pointer;

	template<calling_convention Conv, typename ResolveStrategy, class Ret, class... Args >
	class global_function_pointer<Conv, ResolveStrategy, Ret(Args...)> : public global_pointer<ResolveStrategy>
	{
		using function_type = calling_convention_type<Conv, Ret, Args...>;
	public:
		using global_pointer<ResolveStrategy>::global_pointer;
		~global_function_pointer() = default;

		Ret operator()(Args... args) {
			return reinterpret_cast<function_type>(get_pointer())(args...);
		}
	};
}


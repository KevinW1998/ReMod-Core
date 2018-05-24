
#pragma once

#include <remod/calling_convention.h>

namespace remod::details
{
	template<calling_convention Conv, typename Ret, typename... Args>
	struct convention_type;

	template<typename Ret, typename... Args>
	struct convention_type<calling_convention::conv_stdcall, Ret, Args...>
	{
		using function_type = Ret(__stdcall*)(Args...);
	};

	template<typename Ret, typename... Args>
	struct convention_type<calling_convention::conv_cdecl, Ret, Args...>
	{
		using function_type = Ret(__cdecl*)(Args...);
	};

	template<typename Ret, typename... Args>
	struct convention_type<calling_convention::conv_thiscall, Ret, Args...>
	{
		using function_type = Ret(__thiscall*)(Args...);
	};

	template<typename Ret, typename... Args>
	struct convention_type<calling_convention::conv_fastcall, Ret, Args...>
	{
		using function_type = Ret(__fastcall*)(Args...);
	};
}


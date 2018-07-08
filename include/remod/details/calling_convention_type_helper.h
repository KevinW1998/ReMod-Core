
#pragma once

#include <remod/calling_convention.h>

// TODO: Add information about the underlying conventions (size, params, ect...)

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

	template<typename Ret, typename... Args>
	struct convention_type<calling_convention::conv_default, Ret, Args...>
	{
		using function_type = Ret(*)(Args...);
	};

	template<typename Ret, typename... Args>
	inline Ret call_by_convetion(calling_convention conv, void* ptr, Args... args)
	{
		switch (conv)
		{
		case calling_convention::conv_stdcall: 
			return reinterpret_cast<Ret(__stdcall*)(Args...)>(ptr)(args...);
		case calling_convention::conv_cdecl:
			return reinterpret_cast<Ret(__cdecl*)(Args...)>(ptr)(args...);
		case calling_convention::conv_thiscall:
			return reinterpret_cast<Ret(__thiscall*)(Args...)>(ptr)(args...);
		case calling_convention::conv_fastcall:
			return reinterpret_cast<Ret(__fastcall*)(Args...)>(ptr)(args...);
		default: 
			return reinterpret_cast<Ret(*)(Args...)>(ptr)(args...);
		}
	}
}


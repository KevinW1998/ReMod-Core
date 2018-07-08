#pragma once

#include <remod/calling_convention.h>
#include <remod/registers.h>
#include <initializer_list>


namespace remod::details
{
	template<calling_convention Conv>
	struct convention_information;

	template<>
	struct convention_information<calling_convention::conv_stdcall>
	{
		constexpr static bool callee_cleanups = true;
		constexpr static std::initializer_list<registers> pass_by_register = {};
	};

	template<>
	struct convention_information<calling_convention::conv_cdecl>
	{
		constexpr static bool callee_cleanups = false;
		constexpr static std::initializer_list<registers> pass_by_register = {};
	};

	template<typename Ret, typename... Args>
	struct convention_information<calling_convention::conv_thiscall>
	{
		constexpr static bool callee_cleanups = true;
		constexpr static std::initializer_list<registers> pass_by_register = { registers::ecx };
	};

	template<typename Ret, typename... Args>
	struct convention_information<calling_convention::conv_fastcall>
	{
		constexpr static bool callee_cleanups = true;
		constexpr static std::initializer_list<registers> pass_by_register = { registers::ecx, registers::edx };
	};

	template<typename Ret, typename... Args>
	struct convention_information<calling_convention::conv_default> : convention_information<calling_convention::conv_cdecl>
	{
		using convention_information<calling_convention::conv_cdecl>::callee_cleanups;
		using convention_information<calling_convention::conv_cdecl>::pass_by_register;
	};
}


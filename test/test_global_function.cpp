#include <catch.hpp>
#include "remod/remod_definitions.h"
#include "remod.h"

// Original targets for function tests
int __cdecl global_add_func_cdecl(int a, int b)
{
	return a + b;
}

int __stdcall global_add_func_stdcall(int a, int b)
{
	return a + b;
}

int __fastcall global_add_func_fastcall(int a, int b)
{
	return a + b;
}


// The actual tests below:

// No-Resolve test
remod::global_function_pointer_no_resolve<remod::calling_convention::conv_cdecl, int(int, int)>
global_add_func_ref_cdecl{reinterpret_cast<std::uintptr_t>(&global_add_func_cdecl)};
remod::global_function_pointer_no_resolve<remod::calling_convention::conv_stdcall, int(int, int)>
global_add_func_ref_stdcall{reinterpret_cast<std::uintptr_t>(&global_add_func_stdcall)};
remod::global_function_pointer_no_resolve<remod::calling_convention::conv_fastcall, int(int, int)>
global_add_func_ref_fastcall{reinterpret_cast<std::uintptr_t>(&global_add_func_fastcall)};


TEST_CASE("Test global function pointer without resolve", "[remod-global-function]")
{
	remod::resolve_all();

	// Function check
	REQUIRE(global_add_func_cdecl(1, 2) == 3);
	REQUIRE(global_add_func_ref_stdcall(1, 2) == 3);
	REQUIRE(global_add_func_ref_fastcall(1, 2) == 3);
}


// Win32-specific
#ifdef REMOD_WIN_DETECTED

#include <windows.h>

remod::global_function_pointer_win32<remod::calling_convention::conv_cdecl, int(int, int)>
global_add_func_ref_cdecl_win32{
	reinterpret_cast<std::uintptr_t>(&global_add_func_cdecl) - reinterpret_cast<std::uintptr_t>(GetModuleHandle(nullptr)
	)
};
remod::global_function_pointer_win32<remod::calling_convention::conv_stdcall, int(int, int)>
global_add_func_ref_stdcall_win32{
	reinterpret_cast<std::uintptr_t>(&global_add_func_stdcall) - reinterpret_cast<std::uintptr_t>(
		GetModuleHandle(nullptr))
};
remod::global_function_pointer_win32<remod::calling_convention::conv_fastcall, int(int, int)>
global_add_func_ref_fastcall_win32{
	reinterpret_cast<std::uintptr_t>(&global_add_func_fastcall) - reinterpret_cast<std::uintptr_t>(
		GetModuleHandle(nullptr))
};

TEST_CASE("Test global function pointer with win32 resolve", "[remod-global-function]")
{
	remod::resolve_all();

	REQUIRE(global_add_func_ref_cdecl_win32(1, 2) == 3);
	REQUIRE(global_add_func_ref_stdcall_win32(1, 2) == 3);
	REQUIRE(global_add_func_ref_fastcall_win32(1, 2) == 3);
}

#endif

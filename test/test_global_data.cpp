#include <catch.hpp>
#include "remod/remod_definitions.h"
#include "remod.h"


// Original targets for data tests
struct test_struct_for_data
{
	int a;
	int b;

	void set_for_both(int c, int d)
	{
		a = c;
		b = d;
	}

	int get_sum()
	{
		return a + b;
	}
};


test_struct_for_data global_value_for_no_resolve = { 4, 4 };
test_struct_for_data global_value_for_win32_resolve = { 6, 6 };


// The actual tests below:

remod::global_data_no_resolve<test_struct_for_data> global_value_no_resolve{
	reinterpret_cast<std::uintptr_t>(&global_value_for_no_resolve)
};

TEST_CASE("Test global data pointer without resolve", "[remod-global-data]")
{
	remod::resolve_all();

	test_struct_for_data& global_val_ref = global_value_no_resolve.get_ref();
	// Value check
	REQUIRE(global_val_ref.a == 4);
	REQUIRE(global_val_ref.b == 4);

	global_val_ref.b = 6;

	REQUIRE(global_val_ref.get_sum() == 10);

	// Test original data
	REQUIRE(global_value_for_no_resolve.a == 4);
	REQUIRE(global_value_for_no_resolve.b == 6);
}


// Win32-specific
#ifdef REMOD_WIN_DETECTED

#include <windows.h>

remod::global_data_win32<test_struct_for_data> global_value_win32 {
	reinterpret_cast<std::uintptr_t>(&global_value_for_win32_resolve) - reinterpret_cast<std::uintptr_t>(
		GetModuleHandle(nullptr))
};

TEST_CASE("Test global data pointer with win32 resolve", "[remod-global-data]")
{
	remod::resolve_all();


	test_struct_for_data& global_val_ref = global_value_win32.get_ref();
	REQUIRE(global_val_ref.a == 6);
	REQUIRE(global_val_ref.b == 6);

	global_val_ref.b = 10;

	REQUIRE(global_val_ref.get_sum() == 16);

	// Test original data
	REQUIRE(global_value_for_win32_resolve.a == 6);
	REQUIRE(global_value_for_win32_resolve.b == 10);
}

#endif

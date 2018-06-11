#include <catch.hpp>
#include "remod/remod_definitions.h"
#include "remod.h"


// Original targets for data tests
struct test_struct
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


test_struct global_value_for_ptr_no_resolve = { 4, 4 };
test_struct global_value_for_ptr_win32_resolve = { 6, 6 };

test_struct* global_value_ptr_no_resolve = &global_value_for_ptr_no_resolve;
test_struct* global_value_ptr_win32_resolve = &global_value_for_ptr_win32_resolve;



// The actual tests below:

remod::global_data_pointer_no_resolve<test_struct> global_value_no_resolve{
	reinterpret_cast<std::uintptr_t>(&global_value_ptr_no_resolve)
};

TEST_CASE("Test global data pointer without resolve", "[remod-global-data]")
{
	remod::resolve_all();

	// Value check
	REQUIRE(global_value_no_resolve->a == 4);
	REQUIRE(global_value_no_resolve->b == 4);

	(*global_value_no_resolve).b = 6;

	REQUIRE(global_value_no_resolve->get_sum() == 10);

	// Test original data
	REQUIRE(global_value_for_ptr_no_resolve.a == 4);
	REQUIRE(global_value_for_ptr_no_resolve.b == 6);

}


// Win32-specific
#ifdef REMOD_WIN_DETECTED

#include <windows.h>

remod::global_data_pointer_win32<test_struct> global_value_win32 {
	reinterpret_cast<std::uintptr_t>(&global_value_ptr_win32_resolve) - reinterpret_cast<std::uintptr_t>(
		GetModuleHandle(nullptr))
};

TEST_CASE("Test global data pointer with win32 resolve", "[remod-global-data]")
{
	remod::resolve_all();

	REQUIRE(global_value_win32->a == 6);
	REQUIRE(global_value_win32->b == 6);

	(*global_value_win32).b = 10;

	REQUIRE(global_value_win32->get_sum() == 16);

	// Test original data
	REQUIRE(global_value_for_ptr_win32_resolve.a == 6);
	REQUIRE(global_value_for_ptr_win32_resolve.b == 10);
}

#endif

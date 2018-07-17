#include <catch.hpp>
#include <remod/platform_detection.h>
#include "remod_test_utils.h"
#include "remod/patch_manager.h"
#include "remod/win32/resolve_strategy_win32.h"
#include "remod/resolve_strategy.h"


int REMOD_NOINLINE __cdecl calc_square(int value)
{
	return value * value;
}

int REMOD_NOINLINE calc_func_usage_example()
{
	int calc_result = calc_square(10);
	return calc_result + 1;
}


TEST_CASE("Test trackable_function_patch in general", "[remod-trackable-function-patch]")
{	
	REQUIRE(calc_func_usage_example() == 101);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&calc_func_usage_example));

	// This is the patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	// A stateful-lambda wrapped in std::function
	std::function<int(int)> calc_patch = [](int value)
	{
		return value * 2;
	};

	// detour_point represents the preparation of the patch (here you can defined additional captures, like getting variables from the callers stack)
	remod::detour_point my_detour(to_patch);
	my_detour.init_with_signature(calc_patch); // Here we init the args, which get passed by the caller

	// Test scoping
	{
		// Here apply the patch and add our detour function. my_trackable_patch is RAII protected, so if it gets
		// out of scope, then it gets unpatched.
		auto my_trackable_patch = my_patch_manager.apply(my_detour, calc_patch);

		// Test if the patch is working
		REQUIRE(calc_func_usage_example() == 21);


		// Now try unpatching
		my_trackable_patch->unpatch();
		REQUIRE(calc_func_usage_example() == 101);

		// Now try repatching
		my_trackable_patch->patch();
		REQUIRE(calc_func_usage_example() == 21);

		// Now try getting the return value from source
		my_trackable_patch->set_return_value_source(remod::return_value_source::original_function);
		REQUIRE(calc_func_usage_example() == 101);

		// Now add a new observer
		my_trackable_patch->add_detour_function([](int value)
		{
			REQUIRE(value == 10);
			return 0;
		});

		my_trackable_patch->set_return_value_source(remod::return_value_source::first_detour_function);
		REQUIRE(calc_func_usage_example() == 21);
	} // my_trackable_patch out of scope --> destroyed
	
	// if my_trackable_patch is out of scope and is not detached, then it gets unpatched again
	REQUIRE(calc_func_usage_example() == 101);
}


int REMOD_NOINLINE __fastcall sum_val(int a, int b, int c)
{
	return a + b + c;
}

int REMOD_NOINLINE calc_sum_usage_example()
{
	return sum_val(3, 2, 1);
}

TEST_CASE("Test fastcall", "[remod-trackable-function-patch]")
{
	REQUIRE(calc_sum_usage_example() == 6);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&calc_sum_usage_example));

	auto calc_patch = [](int a, int b, int c)
	{
		return a - b + c;
	};

	// This is the patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	remod::detour_point my_detour(to_patch);
	my_detour.set_convention(remod::calling_convention::conv_fastcall);
	my_detour.init_with_signature(calc_patch); // Here we init the args, which get passed by the caller

	// Apply and add our detour function. You can add multiple detour functions if you want.
	auto my_trackable_patch = my_patch_manager.apply(my_detour, calc_patch);

	REQUIRE(calc_sum_usage_example() == 2);
}





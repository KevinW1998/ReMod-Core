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

	// Here apply the patch. my_trackable_patch is RAII protected, so if it gets
	// out of scope, then it gets unpatched.
	auto my_trackable_patch = my_patch_manager.apply(my_detour, calc_patch);

	// Now add our detour function. You can add multiple detour functions if you want.
	my_trackable_patch->add_detour_function(calc_patch);

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
	calc_func_usage_example();

}


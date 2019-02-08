#include <catch.hpp>
#include <remod/platform_detection.h>
#include "remod_test_utils.h"
#include "remod/patch_manager.h"
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

	// This is the on_patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	// A stateful-lambda wrapped in std::function
	std::function<int(int)> calc_patch = [](int value)
	{
		return value * 2;
	};

	// Test scoping
	{
		// Here apply the on_patch and add our detour function. my_trackable_patch is RAII protected, so if it gets
		// out of scope, then it gets unpatched.
		auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch({ to_patch , remod::signature_from_function(calc_patch) } , calc_patch);

		// Test if the on_patch is working
		REQUIRE(calc_func_usage_example() == 21);


		// Now try unpatching
		my_trackable_patch->on_unpatch();
		REQUIRE(calc_func_usage_example() == 101);

		// Now try repatching
		my_trackable_patch->on_patch();
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

TEST_CASE("Test without adding detour", "[remod-trackable-function-patch]")
{
	REQUIRE(calc_func_usage_example() == 101);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&calc_func_usage_example));

	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	std::function<int(int)> calc_patch = [](int value)
	{
		return value * 2;
	};

	auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch({ to_patch , remod::signature_from_function(calc_patch) }, remod::signature_from_function(calc_patch));

	// Returns 1 because there is no detour function installed
	REQUIRE(calc_func_usage_example() == 1);
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

	// This is the on_patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	// detour_point represents the preparation of the on_patch (here you can defined additional captures, like getting variables from the callers stack)
	remod::detour_point my_detour(to_patch);
	my_detour.set_convention(remod::calling_convention::conv_fastcall);
	my_detour.init_with_signature(remod::signature_from_function(calc_patch)); // Here we init the args, which get passed by the caller

	// Apply and add our detour function. You can add multiple detour functions if you want.
	auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch(my_detour, calc_patch);

	REQUIRE(calc_sum_usage_example() == 2);
}

int REMOD_NOINLINE __stdcall sub_val(int a, int b)
{
	return a - b;
}

int REMOD_NOINLINE calc_sub_usage_example()
{
	return sub_val(3, 2);
}

TEST_CASE("Test stdcall", "[remod-trackable-function-patch]")
{
	REQUIRE(calc_sub_usage_example() == 1);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&calc_sub_usage_example));

	auto calc_patch = [](int a, int b)
	{
		return a + b;
	};

	// This is the on_patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	// detour_point represents the preparation of the on_patch (here you can defined additional captures, like getting variables from the callers stack)
	remod::detour_point my_detour(to_patch);
	my_detour.set_convention(remod::calling_convention::conv_stdcall);
	my_detour.init_with_signature(remod::signature_from_function(calc_patch)); // Here we init the args, which get passed by the caller

	// Apply and add our detour function. You can add multiple detour functions if you want.
	auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch(my_detour, calc_patch);

	REQUIRE(calc_sub_usage_example() == 5);
}

struct big_example_struct
{
	int a;
	float b;
	short c;
};

int REMOD_NOINLINE __cdecl consume_big_example_struct(int some_val_1, big_example_struct data, int some_val_2)
{
	return data.a + static_cast<int>(data.b * 100.f) + data.c + (some_val_1 * some_val_2);
}

int REMOD_NOINLINE use_big_example_struct_function()
{
	// 20 + (0.5 * 100) + 10 + (10 * 5)
	// 20 + 50 + 10 + 50 = 130
	return consume_big_example_struct(10, { 20, 0.5f, 10 }, 5); 
}


/*
TEST_CASE("Test with big struct", "[remod-trackable-function-patch]")
{
	REQUIRE(use_big_example_struct_function() == 130);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&use_big_example_struct_function));

	auto calc_patch = [](int some_val_1, big_example_struct data, int some_val_2) -> int
	{
		// 20 + (0.5 * 10) - 10 + (10 / 5)
		// 20 + 5 - 10 + 2 = 17
		return data.a + static_cast<int>(data.b * 10.f) - data.c + (some_val_1 / some_val_2);
	};

	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;
	auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch({ to_patch , remod::signature_from_function(calc_patch) }, calc_patch);

	REQUIRE(use_big_example_struct_function() == 17);
}

TEST_CASE("Test with invalid fastcall signature", "[remod-trackable-function-on_patch]")
{
	// Just use any function
	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&use_big_example_struct_function));

	remod::function_signature<int(int, big_example_struct, int)> invalid_sig;

	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;
	REQUIRE_THROWS_WITH(my_patch_manager.apply_trackable_function_patch({ to_patch , remod::calling_convention::conv_fastcall, invalid_sig }, invalid_sig), "Second fastcall argument must be equal or smaller than 4 bytes");
}
*/



TEST_CASE("Test with register capture", "[remod-trackable-function-patch]")
{
	void* stack_tmp = nullptr;
	std::intptr_t main_stack_loc = reinterpret_cast<std::intptr_t>(&stack_tmp);

	auto to_patch = remod::test_utils::find_call_small_func(reinterpret_cast<void*>(&sub_val));

	// This is the on_patch manager, holding all patches
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	auto calc_patch = [main_stack_loc](int a, int b, int func_stack_loc)
	{
		REQUIRE(main_stack_loc >= func_stack_loc); // Stack pointer should have shrunk
		return a + b;
	};

	remod::detour_point my_detour(to_patch);
	my_detour.set_convention(remod::calling_convention::conv_stdcall);
	my_detour.add_argument<int>(); // arg b
	my_detour.add_argument<int>(); // arg a
	my_detour.add_capture(remod::register_capture(remod::registers::edx));

	auto my_trackable_patch = my_patch_manager.apply_trackable_function_patch(my_detour, calc_patch);

	REQUIRE(calc_sub_usage_example() == 5);
}




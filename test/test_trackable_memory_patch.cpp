#include <catch.hpp>
#include "remod/resolve_strategy.h"
#include "remod/patch_manager.h"


TEST_CASE("Test memory patch with local variable", "[remod-trackable-memory-patch]")
{
	int simple_value = 4;
	REQUIRE(simple_value == 4);

	{
		remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

		remod::trackable_wrapper<remod::trackable_memory_patch> memory_patch = my_patch_manager.create_trackable_memory_patch();
		memory_patch->set_patch_pointer(reinterpret_cast<std::uintptr_t>(&simple_value));
		memory_patch->set_as_binary_data(int(10));
		REQUIRE(simple_value == 4);

		memory_patch->patch();
		REQUIRE(simple_value == 10);

		memory_patch->unpatch();
		REQUIRE(simple_value == 4);

		memory_patch->patch();
		REQUIRE(simple_value == 10);
	}

	REQUIRE(simple_value == 4);
}

TEST_CASE("Test memory patch with inline", "[remod-trackable-memory-patch]")
{
	int simple_value = 4;
	REQUIRE(simple_value == 4);

	{
		remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

		remod::trackable_wrapper<remod::trackable_memory_patch> memory_patch = my_patch_manager.create_trackable_memory_patch(reinterpret_cast<std::uintptr_t>(&simple_value), 10);
		memory_patch->patch();
		REQUIRE(simple_value == 10);
	}

	REQUIRE(simple_value == 4);
}

static std::uint64_t big_data = 100;

TEST_CASE("Test memory patch with global variable", "[remod-trackable-memory-patch]")
{
	REQUIRE(big_data == 100);

	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	{
		remod::trackable_wrapper<remod::trackable_memory_patch> memory_patch = my_patch_manager.create_trackable_memory_patch();
		memory_patch->set_patch_pointer(reinterpret_cast<std::uintptr_t>(&big_data));
		memory_patch->set_binary_data({ 0xDE, 0xC0, 0xAD, 0xDE, 0xDE, 0xC0, 0xAD, 0xDE }); // Keep endian in mind

		REQUIRE(big_data == 100);

		memory_patch->patch();
		REQUIRE(big_data == 0xDEADC0DEDEADC0DE);
	}
	REQUIRE(big_data == 100);
}

TEST_CASE("Modify memory patch, while patch is active", "[remod-trackable-memory-patch]")
{
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	{
		remod::trackable_wrapper<remod::trackable_memory_patch> memory_patch = my_patch_manager.create_trackable_memory_patch();
		memory_patch->set_patch_pointer(reinterpret_cast<std::uintptr_t>(&big_data));
		memory_patch->set_as_binary_data(std::uint64_t(0));
		memory_patch->patch();

		REQUIRE_THROWS_WITH(memory_patch->set_as_binary_data(std::uint64_t(10)), "Cannot set data if patch is active");
	}
}




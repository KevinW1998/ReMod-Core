#include <catch.hpp>
#include "remod/resolve_strategy.h"
#include "remod/patch_manager.h"


TEST_CASE("Test memory read/write with local variable", "[remod-memory-modification]")
{
	int simple_value = 4;
	REQUIRE(simple_value == 4);

	
	remod::patch_manager<remod::resolve_strategy_noop> my_patch_manager;

	my_patch_manager.write_memory(reinterpret_cast<std::uintptr_t>(&simple_value), 2);
	REQUIRE(simple_value == 2);
		
	simple_value = 3;

	REQUIRE(my_patch_manager.read_memory<int>(reinterpret_cast<std::uintptr_t>(&simple_value)) == 3);
	
}



#pragma once

#include <cstdlib>
#include <cstdint>

namespace remod::test_utils
{
	void* find_call_E8(void* start, std::size_t bytes_search);
	void* resolve_if_jmp_stub(void* func);
	std::uintptr_t find_call_small_func(void* start);
}



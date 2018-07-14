#include "remod_test_utils.h"

#include <algorithm>
#include <stdexcept>

void* remod::test_utils::find_call_E8(void* start, std::size_t bytes_search)
{
	auto begin_search = reinterpret_cast<unsigned char*>(start);
	auto end_search = reinterpret_cast<unsigned char*>(start) + bytes_search;

	auto result = std::find(begin_search, end_search, 0xE8);
	if (result == end_search)
		throw std::runtime_error("Invalid function, no CALL (0xE8) ASM found");

	return result;
}

void* remod::test_utils::resolve_if_jmp_stub(void* func)
{
	auto func_bytes = reinterpret_cast<unsigned char*>(func);

	if(func_bytes[0] == 0xE9)
	{
		std::uintptr_t calling_ptr = reinterpret_cast<std::uintptr_t>(func_bytes);
		auto rel_addr = *reinterpret_cast<std::uintptr_t*>(func_bytes + 1);
		auto abs_addr = calling_ptr + rel_addr + 5;
		return reinterpret_cast<void*>(abs_addr);
	}

	return func;
}

std::uintptr_t remod::test_utils::find_call_small_func(void* start)
{
	return reinterpret_cast<std::uintptr_t>(remod::test_utils::find_call_E8(remod::test_utils::resolve_if_jmp_stub(start), 30));
}

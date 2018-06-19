
#include <remod/base_patch_manager.h>
#include "remod/patch_engine.h"


void remod::base_patch_manager::generate_function_patch(std::uintptr_t addr_for_patch, trackable_patch* patch,
	void* proxy, const detour_point& detour_point_to_apply)
{
	std::uint8_t buf_bytes[10];
	patch_engine::read(addr_for_patch, &buf_bytes, 10u);


	std::uintptr_t orig_addr = 0;

	// Currently we only support relative call patches
	if (buf_bytes[0] == 0xE8)
	{
		std::int32_t relAddr;
		memcpy(&relAddr, &buf_bytes[1], sizeof(std::int32_t));
		orig_addr = addr_for_patch + relAddr + 5;
	} else {
		throw std::runtime_error("Invalid pointer for CALL instruction.");
	}

	// Create stub
	
}

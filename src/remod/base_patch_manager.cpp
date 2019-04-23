
#include <remod/base_patch_manager.h>
#include "remod/patch_engine.h"
#include <stdexcept>

#include "details/asm_code_generator_impl.h"

remod::base_patch_manager::base_patch_manager() : m_generator(std::make_unique<details::asm_code_generator>())
{}

remod::base_patch_manager::~base_patch_manager()
{
}

void remod::base_patch_manager::generate_function_patch(std::uintptr_t addr_for_patch, trackable_function_patch_base* patch,
	void* proxy, const detour_point& detour_point_to_apply)
{
	std::uint8_t buf_bytes[10];
	patch_engine::read(addr_for_patch, &buf_bytes, 10u);


	std::intptr_t orig_addr = 0;
	
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
	std::intptr_t func_stub = m_generator->generator_call_conv_detour_2(detour_point_to_apply, reinterpret_cast<std::intptr_t>(patch), reinterpret_cast<std::intptr_t>(proxy));

	// calculate relative address
	std::int32_t rel_func_stub_addr = func_stub - addr_for_patch - 5;
	memcpy(&buf_bytes[1], &rel_func_stub_addr, sizeof(std::int32_t));

	// TODO: Move to trackable_patch
	// on_patch CALL-Instruction
	patch->set_pointer(addr_for_patch);
	patch->set_data({ buf_bytes, buf_bytes + (1 + sizeof(std::int32_t)) });
	patch->set_calling_convention(detour_point_to_apply.get_calling_convention());

	// Wrap original function if needed
	orig_addr = detour_point_to_apply.has_captures() ? 
		m_generator->generate_orig_func_wrapper(detour_point_to_apply, orig_addr) : orig_addr;
	patch->set_original_function(reinterpret_cast<void*>(orig_addr));


}

void remod::base_patch_manager::write_memory_data(std::uintptr_t offset, void * data, int len)
{
	patch_engine::write(offset, data, len);
}

void remod::base_patch_manager::read_memory_data(std::uintptr_t offset, void * data, int len)
{
	patch_engine::read(offset, data, len);
}

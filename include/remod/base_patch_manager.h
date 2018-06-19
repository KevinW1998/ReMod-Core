#pragma once

#include <memory>

#include "common.h"
#include "details/patch_store.h"
#include "details/asm_code_generator.h"

namespace remod
{
	class detour_point;

	class REMOD_CORE_EXPORT base_patch_manager
	{
		details::patch_store m_patch_store;
		std::unique_ptr<details::asm_code_generator> m_generator;
	protected:

		details::patch_store* get_patch_store()
		{
			return &m_patch_store;
		}

		void generate_function_patch(std::uintptr_t addr_for_patch, trackable_patch* patch, void* proxy, const detour_point& detour_point_to_apply);

	};
}



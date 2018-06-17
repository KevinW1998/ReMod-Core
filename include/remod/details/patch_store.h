#pragma once

#include <unordered_set>

#include "../common.h"
#include "../trackable_patch.h"
#include "set_unique_ptr.h"

namespace remod::details
{
	class REMOD_CORE_EXPORT patch_store
	{
		std::unordered_set<set_unique_ptr<trackable_patch>> m_stored_tracking_patches;
	public:
		patch_store() = default;
		patch_store(const patch_store&) = delete;
		patch_store(patch_store&&) = default;
		patch_store& operator=(const patch_store& other) = delete;
		patch_store& operator=(patch_store&& other) noexcept = default;

		void add_patch(trackable_patch* patch);
		void remove_patch(trackable_patch* patch);
	};
}



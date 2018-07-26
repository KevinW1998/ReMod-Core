#pragma once

#include "../trackable_patch.h"
#include <vector>

namespace remod
{
	class REMOD_CORE_EXPORT trackable_memory_patch_base : public trackable_patch
	{
	friend class base_patch_manager;
		std::uintptr_t m_patch_ptr = 0;
		std::vector<std::uint8_t> m_data;
		std::vector<std::uint8_t> m_orig_data;
	protected:
		void set_data(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data);
		trackable_memory_patch_base() = default;

	public:
		trackable_memory_patch_base(std::uintptr_t patch_ptr, const std::vector<std::uint8_t>& data);
		trackable_memory_patch_base(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data);

		trackable_memory_patch_base(const trackable_memory_patch_base& other) = delete;
		trackable_memory_patch_base(trackable_memory_patch_base&& other) = delete;
		trackable_memory_patch_base& operator=(const trackable_memory_patch_base& other) = delete;
		trackable_memory_patch_base& operator=(trackable_memory_patch_base&& other) noexcept = delete;

		virtual ~trackable_memory_patch_base();

		// TODO: Use trackable_patch
		virtual void on_patch();
		virtual void on_unpatch();
	};
}



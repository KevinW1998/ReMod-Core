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
	private:
		void ensure_unpatched() const;

	protected:
		void set_pointer(std::uintptr_t patch_ptr);
		void set_data(const std::vector<std::uint8_t>& data);
		void set_data(std::vector<std::uint8_t>&& data);
		trackable_memory_patch_base() = default;

		trackable_memory_patch_base(std::uintptr_t patch_ptr, const std::vector<std::uint8_t>& data);
		trackable_memory_patch_base(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data);
	public:
		trackable_memory_patch_base(const trackable_memory_patch_base& other) = delete;
		trackable_memory_patch_base(trackable_memory_patch_base&& other) = delete;
		trackable_memory_patch_base& operator=(const trackable_memory_patch_base& other) = delete;
		trackable_memory_patch_base& operator=(trackable_memory_patch_base&& other) noexcept = delete;

		virtual ~trackable_memory_patch_base();

		virtual void on_patch() override;
		virtual void on_unpatch() override;
	};
}



#pragma once

#include "common.h"
#include <vector>

namespace remod
{
	class REMOD_CORE_EXPORT trackable_patch
	{
		std::uintptr_t m_patch_ptr;
		std::vector<std::uint8_t> m_data;
		std::vector<std::uint8_t> m_orig_data;
		bool m_is_patched = false;
	public:
		trackable_patch(std::uintptr_t patch_ptr, const std::vector<std::uint8_t>& data);
		trackable_patch(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data);

		trackable_patch(const trackable_patch& other) = delete;
		trackable_patch(trackable_patch&& other) = delete;
		trackable_patch& operator=(const trackable_patch& other) = delete;
		trackable_patch& operator=(trackable_patch&& other) noexcept = delete;

		virtual ~trackable_patch() = default;

		// TODO: Use trackable_patch
		virtual void patch();
		virtual void unpatch();
	};
}



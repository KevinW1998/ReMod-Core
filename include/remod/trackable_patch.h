#pragma once

#include "common.h"

namespace remod
{
	class REMOD_CORE_EXPORT trackable_patch
	{
		bool m_is_patched = false;
	protected:
		trackable_patch() = default;
	public:
		trackable_patch(const trackable_patch& other) = delete;
		trackable_patch(trackable_patch&& other) = delete;
		trackable_patch& operator=(const trackable_patch& other) = delete;
		trackable_patch& operator=(trackable_patch&& other) noexcept = delete;

		bool is_patched() const;

		virtual ~trackable_patch();

		// TODO: Move virtual to protected and use wrapper with bool is_patched-guards
		// TODO: Use trackable_patch
		virtual void patch();
		virtual void unpatch();
	};
}



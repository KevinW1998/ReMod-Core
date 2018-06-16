#pragma once

#include <type_traits>
#include "trackable_patch.h"

namespace remod
{
	template<typename T>
	class trackable_wrapper
	{
		T* m_trackable_ptr;
		bool m_is_detached = false;
	public:
		static_assert(std::is_base_of_v<trackable_patch, T> || std::is_same_v<trackable_patch, T>, "T must be trackable_patch");

		// TODO: Add patch_store*
		trackable_wrapper(T* trackable_ptr) : m_trackable_ptr(trackable_ptr) {}
		~trackable_wrapper()
		{
			
		}

		trackable_wrapper(const trackable_wrapper<T>& other) = delete;

		void detach()
		{
			m_is_detached = true;
		}

		void reattach()
		{
			m_is_detached = false;
		}

		bool is_detached() const
		{
			return m_is_detached;
		}
	};
}



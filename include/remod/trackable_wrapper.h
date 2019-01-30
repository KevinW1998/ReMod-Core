#pragma once

#include <type_traits>
#include "trackable_patch.h"
#include "details/patch_store.h"

namespace remod
{
	/**
	 * \brief A smart pointer, which manages the lifetime of a on_patch (unless it is detached). 
	 * \tparam T Concrete type of trackable_patch. T must be trackable_patch or a derived class of it
	 */
	template<typename T>
	class trackable_wrapper
	{
		details::patch_store* m_internal_store;
		T* m_trackable_ptr;
		bool m_is_detached = false;
	public:
		static_assert(std::is_base_of_v<trackable_patch, T> || std::is_same_v<trackable_patch, T>, "T must be trackable_patch");

		trackable_wrapper() : m_internal_store(nullptr), m_trackable_ptr(nullptr) {}
		trackable_wrapper(details::patch_store* internal_store, T* trackable_ptr) : m_internal_store(internal_store), m_trackable_ptr(trackable_ptr) {}
		~trackable_wrapper()
		{
			if (!m_is_detached && m_trackable_ptr)
				m_internal_store->remove_patch(m_trackable_ptr);
		}

		trackable_wrapper(const trackable_wrapper<T>& other) = delete;
		trackable_wrapper(trackable_wrapper<T>&& other) noexcept
		{
			m_internal_store = other.m_internal_store;
			m_trackable_ptr = other.m_trackable_ptr;
			m_is_detached = other.m_is_detached;

			other.m_trackable_ptr = nullptr;
		}


		trackable_wrapper& operator=(const trackable_wrapper& other) = delete;
		trackable_wrapper& operator=(trackable_wrapper&& other) noexcept
		{
			if (this == &other)
				return *this;
			m_internal_store = other.m_internal_store;
			m_trackable_ptr = other.m_trackable_ptr;
			m_is_detached = other.m_is_detached;

			other.m_trackable_ptr = nullptr;

			return *this;
		}

		T* operator->()
		{
			if (!m_trackable_ptr)
				throw std::runtime_error("Invalid pointer");

			return m_trackable_ptr;
		}

		T& operator* ()
		{
			if (!m_trackable_ptr)
				throw std::runtime_error("Invalid pointer");

			return *m_trackable_ptr;
		}

		explicit operator bool() const { 
			return m_trackable_ptr != nullptr;
		}


		/**
		 * \brief Detaches the internal pointer, so that it is not garbage collected
		 */
		void detach()
		{
			m_is_detached = true;
		}

		
		/**
		 * \brief Reattaches the internal pointer, so that it is garbage collected
		 */
		void reattach()
		{
			m_is_detached = false;
		}


		/**
		 * \brief Returns whether 
		 * \return True, if the pointer is garbage collected
		 */
		bool is_detached() const
		{
			return m_is_detached;
		}
	};
}



#pragma once

#include "details/trackable_memory_patch_base.h"
#include <vector>
#include <iterator>
#include <type_traits>

namespace remod
{
	class REMOD_CORE_EXPORT trackable_memory_patch : public trackable_memory_patch_base
	{
	public:
		void set_patch_pointer(std::uintptr_t patch_ptr);
		void set_binary_data(const std::vector<std::uint8_t>& data);
		void set_binary_data(std::vector<std::uint8_t>&& data);

		template<typename T>
		void set_as_binary_data(T value)
		{
			constexpr static const size_t SizeOfT = sizeof(T);
			static_assert(std::is_pod_v<T>, "T must be a pod so it can be used safely as binary data");

			std::uint8_t* begin_ptr = reinterpret_cast<std::uint8_t*>(&value);
			std::uint8_t* end_ptr = begin_ptr + SizeOfT;

			std::vector<std::uint8_t> binary_data;
			binary_data.reserve(SizeOfT);
			std::copy(begin_ptr, end_ptr, std::back_inserter(binary_data));

			set_binary_data(std::move(binary_data));
		}
	};
}



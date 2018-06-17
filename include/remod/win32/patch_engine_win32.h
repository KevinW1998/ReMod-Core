#pragma once

#include "../common.h"
#include <cstdint>
#include <cstdlib>

namespace remod::win32
{
	class REMOD_CORE_EXPORT patch_engine_win32
	{
	public:
		static void write(std::uintptr_t ptr, void* data, std::size_t size);
		static void read(std::uintptr_t ptr, void* data, std::size_t size);
	};
}


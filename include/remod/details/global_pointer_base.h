#pragma once

#include <cstdint>

#pragma warning(push)
#pragma warning(disable: 4820 4265) // unreferenced parameters

namespace remod::details
{
	class global_pointer_base
	{
		std::uintptr_t m_ptr;
		bool m_resolved = false;
	protected:
		global_pointer_base(std::uintptr_t ptr);
		~global_pointer_base();

	public:
		bool resolve();

		std::uintptr_t get_pointer() const;

		bool is_resolved() const;
	protected:
		std::uintptr_t& get_pointer_raw_ref();

		virtual bool resolve_pointer() = 0;
	};
}

#pragma warning(pop)

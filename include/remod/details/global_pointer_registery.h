#pragma once

#include <unordered_set>

namespace remod::details
{
	class global_pointer_base;

	class global_pointer_registry
	{
		// Data members
		std::unordered_set<global_pointer_base*> m_bases;
	public:
		static global_pointer_registry& get_registry();

		void insert(global_pointer_base* base);

		void remove(global_pointer_base* base);

		bool resolve_all();
	private:
		global_pointer_registry() = default;

	public:
		global_pointer_registry(global_pointer_registry const&) = delete;
		global_pointer_registry(global_pointer_registry&&) = delete;
		void operator=(global_pointer_registry const&) = delete;
		void operator=(global_pointer_registry&&) = delete;
	};
}

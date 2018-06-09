#pragma once

namespace remod
{
	class module {
		const char* m_module_name;
	public:
		constexpr module(const char* module_name) : 
			m_module_name(module_name)
		{}

		constexpr bool is_main_module() const
		{
			return m_module_name == nullptr;
		}

		constexpr const char* get_module_name() const
		{
			return m_module_name;
		}
	};

	constexpr const module main_module(nullptr);
}



#pragma once

namespace remod
{
	class module {
		const char* m_module_name;
		bool m_has_module_addr_hint;
		std::uintptr_t m_module_hint;
	public:
		constexpr module(const char* module_name) : 
			m_module_name(module_name),
			m_has_module_addr_hint(false),
			m_module_hint(0)
		{}

		constexpr module(const char* module_name, std::uintptr_t module_hint) :
			m_module_name(module_name),
			m_has_module_addr_hint(true),
			m_module_hint(module_hint)
		{}

		constexpr bool is_main_module() const
		{
			return m_module_name == nullptr;
		}

		constexpr bool has_module_hint() const
		{
			return m_has_module_addr_hint;
		}

		constexpr std::uintptr_t get_module_hint() const
		{
			return m_module_hint;
		}

		constexpr const char* get_module_name() const
		{
			return m_module_name;
		}
	};

	constexpr const module main_module(nullptr);
}



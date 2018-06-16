
#include <remod/captures.h>

remod::base_offset_capture::base_offset_capture(std::uintptr_t offset) : m_offset(offset) 
{}

std::uintptr_t remod::base_offset_capture::get_offset() const
{
	return m_offset;
}

remod::register_capture::register_capture(registers selected_register) : m_selected_register(selected_register)
{}

remod::registers remod::register_capture::get_register() const
{
	return m_selected_register;
}


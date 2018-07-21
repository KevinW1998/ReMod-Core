
#include <remod/captures.h>

remod::base_capture::base_capture(std::size_t capture_size) : m_capture_size(capture_size)
{}


remod::register_capture::register_capture(registers selected_register) : base_capture(4), m_selected_register(selected_register)
{}

std::size_t remod::base_capture::get_capture_size() const
{
	return m_capture_size;
}

remod::base_offset_capture::base_offset_capture(std::uintptr_t offset, std::size_t capture_size) : base_capture(capture_size), m_offset(offset)
{}

std::uintptr_t remod::base_offset_capture::get_offset() const
{
	return m_offset;
}


remod::registers remod::register_capture::get_register() const
{
	return m_selected_register;
}


#include <remod/trackable_patch.h>

#include <remod/patch_engine.h>

void remod::trackable_patch::set_data(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data)
{
	m_patch_ptr = patch_ptr;
	m_data = std::move(data);
	m_orig_data = std::vector<std::uint8_t>(m_data.size(), '\0');
}

remod::trackable_patch::trackable_patch(std::uintptr_t patch_ptr, const std::vector<std::uint8_t>& data) : 
	m_patch_ptr(patch_ptr), 
	m_data(data),
	m_orig_data(data.size(), '\0')
{}

remod::trackable_patch::trackable_patch(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data) : 
	m_patch_ptr(patch_ptr), 
	m_data(std::move(data)),
	m_orig_data(data.size(), '\0')
{}

void remod::trackable_patch::patch()
{
	if (m_is_patched)
		return;

	patch_engine::read(m_patch_ptr, reinterpret_cast<void*>(&m_orig_data[0]), m_orig_data.size());
	patch_engine::write(m_patch_ptr, reinterpret_cast<void*>(&m_data[0]), m_data.size());

	m_is_patched = true;
}

void remod::trackable_patch::unpatch()
{
	if (!m_is_patched)
		return;

	patch_engine::write(m_patch_ptr, reinterpret_cast<void*>(&m_orig_data[0]), m_orig_data.size());

	m_is_patched = false;
}



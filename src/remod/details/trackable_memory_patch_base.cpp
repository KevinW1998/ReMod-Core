#include <remod/details/trackable_memory_patch_base.h>

#include <remod/patch_engine.h>

void remod::trackable_memory_patch_base::ensure_unpatched() const
{
	if (is_patched())
		throw std::runtime_error("Cannot set data if patch is active");
}

void remod::trackable_memory_patch_base::set_pointer(std::uintptr_t patch_ptr)
{
	ensure_unpatched();

	m_patch_ptr = patch_ptr;
}

void remod::trackable_memory_patch_base::set_data(const std::vector<std::uint8_t>& data)
{
	ensure_unpatched();

	m_data = data;
	m_orig_data = std::vector<std::uint8_t>(m_data.size(), '\0');
}

void remod::trackable_memory_patch_base::set_data(std::vector<std::uint8_t>&& data)
{
	ensure_unpatched();

	m_data = std::move(data);
	m_orig_data = std::vector<std::uint8_t>(m_data.size(), '\0');
}

remod::trackable_memory_patch_base::trackable_memory_patch_base(std::uintptr_t patch_ptr, const std::vector<std::uint8_t>& data) : 
	m_patch_ptr(patch_ptr), 
	m_data(data),
	m_orig_data(data.size(), '\0')
{}

remod::trackable_memory_patch_base::trackable_memory_patch_base(std::uintptr_t patch_ptr, std::vector<std::uint8_t>&& data) : 
	m_patch_ptr(patch_ptr), 
	m_data(std::move(data)),
	m_orig_data(data.size(), '\0')
{}

remod::trackable_memory_patch_base::~trackable_memory_patch_base()
{
	if (is_patched())
		trackable_memory_patch_base::on_unpatch();
}

void remod::trackable_memory_patch_base::on_patch()
{
	patch_engine::read(m_patch_ptr, reinterpret_cast<void*>(&m_orig_data[0]), m_orig_data.size());
	patch_engine::write(m_patch_ptr, reinterpret_cast<void*>(&m_data[0]), m_data.size());
}

void remod::trackable_memory_patch_base::on_unpatch()
{
	patch_engine::write(m_patch_ptr, reinterpret_cast<void*>(&m_orig_data[0]), m_orig_data.size());
}



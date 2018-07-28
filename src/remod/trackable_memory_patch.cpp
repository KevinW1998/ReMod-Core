
#include <remod/trackable_memory_patch.h>


void remod::trackable_memory_patch::set_patch_pointer(std::uintptr_t patch_ptr)
{
	set_pointer(patch_ptr);
}

void remod::trackable_memory_patch::set_binary_data(const std::vector<std::uint8_t>& data)
{
	set_data(data);
}

void remod::trackable_memory_patch::set_binary_data(std::vector<std::uint8_t>&& data)
{
	set_data(std::move(data));
}

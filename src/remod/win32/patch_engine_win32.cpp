
#include <remod/win32/patch_engine_win32.h>

#include <memoryapi.h>
#include <stdexcept>

// Maybe export to another class?
class win32_virtual_protect
{
	std::uintptr_t m_ptr;
	std::size_t m_size;
	DWORD m_old_protect = PAGE_EXECUTE_READ; // Default for code, normally

public:
	win32_virtual_protect(std::uintptr_t ptr, std::size_t size, DWORD protection = PAGE_EXECUTE_READWRITE) : m_ptr(ptr), m_size(size)
	{
		auto result = VirtualProtect(reinterpret_cast<LPVOID>(ptr), size, protection, &m_old_protect);
		if(!result)
		{
			throw std::runtime_error("Failed to change protection for write access");
		}
	}

	~win32_virtual_protect()
	{
		// we omit the error code result, because throwing in dtors is a no-no
		VirtualProtect(reinterpret_cast<LPVOID>(m_ptr), m_size, m_old_protect, &m_old_protect);
	}
};


void remod::win32::patch_engine_win32::write(std::uintptr_t ptr, void* data, std::size_t size)
{
	win32_virtual_protect raii_protect(ptr, size, PAGE_EXECUTE_READWRITE);

	memcpy(reinterpret_cast<void*>(ptr), data, size);
}

void remod::win32::patch_engine_win32::read(std::uintptr_t ptr, void* data, std::size_t size)
{
	win32_virtual_protect raii_protect(ptr, size, PAGE_EXECUTE_READ);

	memcpy(data, reinterpret_cast<void*>(ptr), size);
}

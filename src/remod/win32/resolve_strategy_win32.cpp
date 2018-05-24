
#include <remod/win32/resolve_strategy_win32.h>
#include <remod/module.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


bool remod::win32::resolve_strategy_module_handle::resolve(std::uintptr_t& to_resolve, const remod::module& module)
{
	const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(module.get_module_name()));
	if (base == 0)
		return false;

	to_resolve += base;
	return true;
}





#include <remod/resolve_strategy.h>
#include <remod/module.h>

#pragma warning(disable: 4100) // unreferenced parameters

bool remod::resolve_strategy_noop::resolve(std::uintptr_t& to_resolve, const module& module)
{
	return true;
}

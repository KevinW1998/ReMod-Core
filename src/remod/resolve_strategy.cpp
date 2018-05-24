
#include <remod/resolve_strategy.h>

#include <remod/module.h>

bool remod::resolve_strategy_noop::resolve(std::uintptr_t& to_resolve, const module& module)
{
	return true;
}

bool remod::resolve_strategy_static::resolve(std::uintptr_t& to_resolve, const module& module)
{
	if(!module.has_module_hint())
		return false;

	to_resolve += module.get_module_hint();
	return true;
}

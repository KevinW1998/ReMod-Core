
#include <remod/details/global_pointer_registery.h>
#include <remod/details/global_pointer_base.h>

remod::details::global_pointer_registry& remod::details::global_pointer_registry::get_registry()
{
	static global_pointer_registry instance;

	return instance;
}

void remod::details::global_pointer_registry::insert(global_pointer_base* base)
{
	m_bases.insert(base);
}

void remod::details::global_pointer_registry::remove(global_pointer_base* base)
{
	m_bases.erase(base);
}

bool remod::details::global_pointer_registry::resolve_all()
{
	for (auto base : m_bases)
	{
		if (!base->resolve())
			return false;
	}
	return true;
}


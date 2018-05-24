
#include <remod/details/global_pointer_base.h>
#include <remod/details/global_pointer_registery.h>
#include <cassert>


remod::details::global_pointer_base::global_pointer_base(std::uintptr_t ptr): m_ptr(ptr)
{
	global_pointer_registry::get_registry().insert(this);
}

remod::details::global_pointer_base::~global_pointer_base()
{
	global_pointer_registry::get_registry().remove(this);
}

bool remod::details::global_pointer_base::resolve()
{
	if (m_resolved)
		return true;

	if (!resolve_pointer())
		return false;
	m_resolved = true;
	return true;
}

std::uintptr_t remod::details::global_pointer_base::get_pointer() const
{
	assert(is_resolved());

	return m_ptr;
}

bool remod::details::global_pointer_base::is_resolved() const
{
	return m_resolved;
}

std::uintptr_t& remod::details::global_pointer_base::get_pointer_raw_ref()
{
	return m_ptr;
}



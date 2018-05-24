
#include <remod/remod_functions.h>
#include <remod/details/global_pointer_registery.h>

void remod::resolve_all()
{
	details::global_pointer_registry::get_registry().resolve_all();
}

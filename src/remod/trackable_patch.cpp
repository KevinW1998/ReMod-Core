#include <remod/trackable_patch.h>

#include <remod/patch_engine.h>

bool remod::trackable_patch::is_patched() const
{
	return m_is_patched;
}

void remod::trackable_patch::patch()
{
	if (m_is_patched)
		return;

	on_patch();

	m_is_patched = true;
}

void remod::trackable_patch::unpatch()
{
	if (!m_is_patched)
		return;

	on_unpatch();

	m_is_patched = false;
}

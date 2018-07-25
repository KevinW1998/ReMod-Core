#include <remod/trackable_patch.h>

#include <remod/patch_engine.h>

bool remod::trackable_patch::is_patched() const
{
	return m_is_patched;
}

remod::trackable_patch::~trackable_patch()
{
	if (m_is_patched)
		trackable_patch::unpatch();
}

void remod::trackable_patch::patch()
{
	if (m_is_patched)
		return;

	m_is_patched = true;
}

void remod::trackable_patch::unpatch()
{
	if (!m_is_patched)
		return;

	m_is_patched = false;
}



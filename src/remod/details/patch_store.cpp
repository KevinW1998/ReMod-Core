
#include <remod/details/patch_store.h>


void remod::details::patch_store::add_patch(trackable_patch* patch)
{
	m_stored_tracking_patches.insert(set_unique_ptr<trackable_patch>(patch));
}

void remod::details::patch_store::remove_patch(trackable_patch* patch)
{
	m_stored_tracking_patches.erase(make_find_ptr(patch));
}

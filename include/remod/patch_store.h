#pragma once

#include <unordered_set>
#include "trackable_patch.h"

namespace remod
{
	class patch_store
	{
		std::unordered_set<trackable_patch*> m_storedTrackingPatches;
	public:
		~patch_store()
		{
			
		}

		void add_patch(trackable_patch* patch);
		void remove_patch(trackable_patch* patch);

	};
}



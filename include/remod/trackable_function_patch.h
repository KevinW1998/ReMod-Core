#pragma once

#include "trackable_patch.h"

namespace remod
{
	template<typename>
	class trackable_function_patch;
	
	template<typename Ret, typename... Args>
	class trackable_function_patch<Ret(Args...)> : trackable_patch {
		
	public:

	};


}



#pragma once

namespace remod::details
{
	inline int ceil_to_4(int val)
	{
		return val <= 4 ? 4 : val;
	}
}


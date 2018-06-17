#pragma once

namespace remod
{
	class trackable_patch
	{
	public:
		virtual ~trackable_patch() = default;

		// TODO: Use trackable_patch
		virtual void patch() {};
		virtual void unpatch() {};


	};
}



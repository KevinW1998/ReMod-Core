#pragma once

namespace remod
{
	class trackable_patch
	{
	public:
		virtual ~trackable_patch() = default;

		virtual void patch() {};
		virtual void unpatch() {};


	};
}



#pragma once

namespace remod
{
	template<typename>
	class function_signature;
	
	template<typename Ret, typename... Args>
	class function_signature<Ret(Args...)> {
		
	public:

	};
}



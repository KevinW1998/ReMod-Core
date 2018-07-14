#pragma once

namespace remod
{
	template<typename>
	struct function_signature;
	
	template<typename Ret, typename... Args>
	struct function_signature<Ret(Args...)> {};
}



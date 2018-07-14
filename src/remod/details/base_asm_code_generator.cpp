
#include "base_asm_code_generator.h"

asmjit::JitRuntime& remod::details::base_asm_code_generator::get_runtime()
{
	return m_runtime;
}

int remod::details::base_asm_code_generator::calculate_stack_to_manage(calling_convention conv,
	const std::vector<size_t>& args)
{
	std::size_t params_to_pop = 0;
	if (conv == calling_convention::conv_thiscall)
		params_to_pop = 1;
	if (conv == calling_convention::conv_fastcall)
		params_to_pop = 2;

	int total_size = 0;
	for(std::size_t i = 0; i < args.size(); ++i)
	{
		if (params_to_pop && args.size() <= 4)
		{
			params_to_pop--;
		}
		else
		{
			std::size_t next_arg_size = args[i];
			total_size += next_arg_size <= 4 ? 4 : next_arg_size;
		}
	}

	return total_size;
}

#pragma once
#include "base_asm_code_generator.h"
#include "remod/calling_convention.h"
#include "remod/detour_point.h"

namespace remod::details
{
	class asm_code_generator_x86 : base_asm_code_generator
	{
	public:

		void validate_args_by_call_conv(const std::vector<std::size_t>& arg_sizes, calling_convention call_conv);
		void generate_call_conv_prologue(asmjit::X86Assembler& assembler, calling_convention source_call_conv, int arg_num);
		asmjit::X86Gp convert_remod_register(registers reg);
		uint32_t convert_remod_call_conv(calling_convention convetion);

		void add_arg_to_dyn_sig(asmjit::FuncSignatureX& sig, std::size_t sizeOfReg);
		void add_args_to_dyn_sig(asmjit::FuncSignatureX& sig, const std::vector<std::size_t>& arg_sizes);

		
		// Stable API:
		// std::intptr_t generator_call_conv_detour(const detour_point& to_convert, std::intptr_t context_value, std::intptr_t func_to_call);
		std::intptr_t generator_call_conv_detour_2(const detour_point& to_convert, std::intptr_t context_value, std::intptr_t func_to_call);
		std::intptr_t generate_orig_func_wrapper(const detour_point& to_convert, std::intptr_t func_to_call);
	};
}


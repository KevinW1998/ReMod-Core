
#include "asm_code_generator_x86.h"


void remod::details::asm_code_generator_x86::validate_args_by_call_conv(const std::vector<std::size_t>& arg_sizes,
	calling_convention call_conv)
{
	int num_of_args = arg_sizes.size();

	if (call_conv == calling_convention::conv_fastcall)
	{
		if (num_of_args >= 1)
		{
			if (arg_sizes[0] > 4)
			{
				throw std::runtime_error("First fastcall argument must be equal or smaller than 4 bytes");
			}
		}
		else if(num_of_args >= 2)
		{
			if (arg_sizes[1] > 4)
			{
				throw std::runtime_error("Second fastcall argument must be equal or smaller than 4 bytes");
			}
		}
	} 
	else if(call_conv == calling_convention::conv_thiscall)
	{
		if(num_of_args >= 1)
		{
			if (arg_sizes[0] > 4)
			{
				throw std::runtime_error("First thiscall argument must be equal or smaller than 4 bytes");
			}
		}
	}
}

void remod::details::asm_code_generator_x86::generate_call_conv_prologue(asmjit::X86Assembler& assembler, calling_convention source_call_conv, int arg_num)
{
	if(source_call_conv == calling_convention::conv_fastcall) // fastcall --> cdecl
	{
		if (arg_num >= 1) 
			assembler.push(asmjit::x86::edx);
		if (arg_num >= 2)
			assembler.push(asmjit::x86::ecx);
	} 
	else if (source_call_conv == calling_convention::conv_thiscall) // thiscall --> cdecl
	{
		if (arg_num >= 1)
			assembler.push(asmjit::x86::ecx);
	}
}

void* remod::details::asm_code_generator_x86::generator_call_conv_detour(detour_point to_convert, void* context_value, void* func_to_call)
{
	// Grab the information we need
	calling_convention source_call_conv = to_convert.get_calling_convention();
	const auto& arg_sizes = to_convert.get_arg_sizes();
	int arg_num = arg_sizes.size();
	int stack_to_manage = calculate_stack_to_manage(source_call_conv, arg_sizes);
	
	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	// Init assembler
	asmjit::X86Assembler a(&code);

	// First validate the arguments by signature
	validate_args_by_call_conv(to_convert.get_arg_sizes(), source_call_conv);

	// TODO: Check https://en.wikibooks.org/wiki/X86_Disassembly/Functions_and_Stack_Frames
	// Then push the args 
	for (int i = 0; i < stack_to_manage; i += 4)
		a.push(asmjit::x86::ptr(asmjit::x86::esp, i + 4));

	// push the args by calling convention
	generate_call_conv_prologue(a, source_call_conv, arg_num);

	// Last
	a.push(reinterpret_cast<int>(context_value));
	a.call(reinterpret_cast<std::intptr_t>(func_to_call));

	if(source_call_conv == calling_convention::conv_cdecl)
	{
		a.ret(4);
	}
	else
	{
		a.ret(stack_to_manage + 4); // Args [with fastcall or thiscall reg args omitted] + context_value
	}
	
	void* fn = nullptr;
	asmjit::Error err = get_runtime().add(&fn, &code);

	return fn;
}

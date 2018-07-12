
#include "asm_code_generator_x86.h"


void remod::details::asm_code_generator_x86::validate_args_by_call_conv(const std::vector<std::size_t>& arg_sizes,
	calling_convention call_conv)
{
	if (call_conv == calling_convention::conv_fastcall)
	{
		if (arg_sizes.size() >= 1)
		{
			if (arg_sizes[0] > 4)
			{
				throw std::runtime_error("First fastcall argument must be equal or smaller than 4 bytes");
			}
		}
		else if(arg_sizes.size() >= 2)
		{
			if (arg_sizes[1] > 4)
			{
				throw std::runtime_error("Second fastcall argument must be equal or smaller than 4 bytes");
			}
		}
	} 
	else if(call_conv == calling_convention::conv_thiscall)
	{
		if (arg_sizes[0] > 4)
		{
			throw std::runtime_error("First thiscall argument must be equal or smaller than 4 bytes");
		}
	}
}

void* remod::details::asm_code_generator_x86::generator_call_conv_detour(detour_point to_convert, void* context_value, void* func_to_call)
{
	// Grab the information we need
	calling_convention source_call_conv = to_convert.get_calling_convention();
	const auto& arg_sizes = to_convert.get_arg_sizes();

	
	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	// Init assembler
	asmjit::X86Assembler a(&code);


	validate_args_by_call_conv(to_convert.get_arg_sizes(), source_call_conv);
	// TODO: Extract prologue to own function (i.e. transform_call_conv_prologue)
	if(source_call_conv == calling_convention::conv_fastcall) // fastcall --> cdecl
	{
		a.push(asmjit::x86::edx); // TODO: Only push if needed
		a.push(asmjit::x86::ecx); // TODO: Only push if needed
	} 
	else if (source_call_conv == calling_convention::conv_thiscall) // thiscall --> cdecl
	{
		a.push(asmjit::x86::ecx); // TODO: Only push if needed
	}

	// TODO: Push extra arguments
	// Last
	a.push(reinterpret_cast<int>(context_value));
	a.call(reinterpret_cast<std::intptr_t>(func_to_call));

	// Now calculate the stack to cleanup
	int stack_to_cleanup = calculate_stack_cleanup_size(source_call_conv, arg_sizes);
	// TODO: ASM sub with cleanup --> for cdecl

	if(source_call_conv == calling_convention::conv_cdecl)
	{
		a.ret(4); // Only cleanup context_value
	} 
	else
	{
		a.ret(stack_to_cleanup + 4); // Args [with fastcall or thiscall reg args omitted] + context_value
	}

	void* fn = nullptr;
	asmjit::Error err = get_runtime().add(&fn, &code);

	return fn;
}

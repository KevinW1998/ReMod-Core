
#include "asm_code_generator_x86.h"
#include <numeric>

#include <remod/details/overloaded.h>
#include "asm_code_helper.h"

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
		if (num_of_args >= 2)
		{
			if (arg_sizes[1] > 4)
			{
				throw std::runtime_error("Second fastcall argument must be equal or smaller than 4 bytes");
			}
		}
	} 
	else if (call_conv == calling_convention::conv_thiscall)
	{
		if (num_of_args >= 1)
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

asmjit::X86Gp remod::details::asm_code_generator_x86::convert_remod_register(registers reg)
{
	if (is_x64_register(reg))
		throw std::runtime_error("Invalid register for x86");

	if (reg == registers::eip)
		throw std::runtime_error("Cannot directly convert EIP");

	switch(reg)
	{
	case registers::ah: return asmjit::x86::ah;
	case registers::al: return asmjit::x86::al;
	case registers::ch: return asmjit::x86::ch;
	case registers::cl: return asmjit::x86::cl;
	case registers::dh: return asmjit::x86::dh;
	case registers::dl: return asmjit::x86::dl;
	case registers::bh: return asmjit::x86::bh;
	case registers::bl: return asmjit::x86::bl;
	case registers::ax: return asmjit::x86::ax;
	case registers::cx: return asmjit::x86::cx;
	case registers::dx: return asmjit::x86::dx;
	case registers::bx: return asmjit::x86::bx;
	case registers::sp: return asmjit::x86::sp;
	case registers::bp: return asmjit::x86::bp;
	case registers::si: return asmjit::x86::si;
	case registers::di: return asmjit::x86::di;
	case registers::eax: return asmjit::x86::eax;
	case registers::ecx: return asmjit::x86::ecx;
	case registers::edx: return asmjit::x86::edx;
	case registers::ebx: return asmjit::x86::ebx;
	case registers::esp: return asmjit::x86::esp;
	case registers::ebp: return asmjit::x86::ebp;
	case registers::esi: return asmjit::x86::esi;
	case registers::edi: return asmjit::x86::edi;
	default: 
		return {};
	}
}

std::intptr_t remod::details::asm_code_generator_x86::generator_call_conv_detour(const detour_point& to_convert, std::intptr_t context_value, std::intptr_t func_to_call)
{
	// Grab the information we need
	calling_convention source_call_conv = to_convert.get_calling_convention();
	source_call_conv = source_call_conv == calling_convention::conv_default ? calling_convention::conv_cdecl : source_call_conv; // default --> cdecl

	const auto& arg_sizes = to_convert.get_arg_sizes();
	int arg_num = arg_sizes.size();
	// This is the argument stack from the caller
	int stack_to_manage = calculate_stack_to_manage(source_call_conv, arg_sizes);
	// This is the argument stack we have redirected
	int detour_stack_to_manage = std::accumulate(arg_sizes.begin(), arg_sizes.end(), 0, [](std::size_t cur, std::size_t val) {return cur + ceil_to_4(val); }); 
	
	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	// Init assembler
	asmjit::X86Assembler a(&code);

	// First validate the arguments by signature
	validate_args_by_call_conv(to_convert.get_arg_sizes(), source_call_conv);

	// TODO: Check https://en.wikibooks.org/wiki/X86_Disassembly/Functions_and_Stack_Frames
	// Then push the args 
	asmjit::X86Mem push_ptr = asmjit::x86::dword_ptr(asmjit::x86::esp, stack_to_manage);
	for (int i = 0; i < stack_to_manage; i += 4)
		a.push(push_ptr);

	// Then push captures
	for(const auto& captures : to_convert.get_captures())
	{
		std::visit(overloaded {
			[&](const stack_capture& stack_captures)
			{
				// TODO: Impl
				// TODO: Check if detour_point caller stack size does fit
				// detour_stack_to_manage += stack_captures.get_capture_size();
			},
			[&](const argument_capture& argument_captures)
			{
				// TODO: Impl
				// detour_stack_to_manage += argument_captures.get_capture_size();
			},
			[&](const register_capture& register_captures)
			{
				a.push(convert_remod_register(register_captures.get_register()));
				detour_stack_to_manage += register_captures.get_capture_size();
			}
		}, captures);
	}

	// push the args by calling convention
	generate_call_conv_prologue(a, source_call_conv, arg_num);

	// Last
	a.push(context_value);
	a.call(func_to_call);

	// TODO: Could be merged with ret
	// Cleanup
	a.add(asmjit::x86::esp, detour_stack_to_manage + 4);

	if (source_call_conv != calling_convention::conv_cdecl)
		a.ret(stack_to_manage);
	else
		a.ret();

	
	std::intptr_t fn = 0;
	asmjit::ErrorCode err = static_cast<asmjit::ErrorCode>(get_runtime().add(&fn, &code));
	if(err)
		throw std::runtime_error("Failed to generate code for detour");

	return fn;
}

std::intptr_t remod::details::asm_code_generator_x86::generate_orig_func_wrapper(const detour_point& to_convert,
	std::intptr_t func_to_call)
{
	// TODO / FIXME: Check calling convention with registers --> Fastcall / Thiscall

	// TODO: Re-Push everything fully

	int size_of_captures = 0;
	for (const auto& next_captures : to_convert.get_captures())
	{
		std::visit([&size_of_captures](const auto& capture)
		{
			size_of_captures += capture.get_capture_size();
		}, next_captures);
	}

	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.
	asmjit::X86Assembler a(&code); // Init assembler

	// a.int3();
	a.add(asmjit::x86::esp, size_of_captures); // Remove args from stack
	a.call(func_to_call);
	if(to_convert.get_calling_convention() == calling_convention::conv_cdecl)
		a.sub(asmjit::x86::esp, size_of_captures); // Add args to stack again
	a.ret();

	std::intptr_t fn = 0;
	asmjit::ErrorCode err = static_cast<asmjit::ErrorCode>(get_runtime().add(&fn, &code));
	if (err)
		throw std::runtime_error("Failed to generate code for detour redirect correction function");

	return fn;
}


#include "asm_code_generator_x86.h"
#include <numeric>
#include <stdexcept>

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

uint32_t remod::details::asm_code_generator_x86::convert_remod_call_conv(calling_convention convention)
{
	switch (convention) {
	case calling_convention::conv_stdcall: return asmjit::CallConv::kIdHostStdCall;
	case calling_convention::conv_cdecl: return asmjit::CallConv::kIdHostCDecl;
	case calling_convention::conv_thiscall: return asmjit::CallConv::kIdX86MsThisCall; // TODO: Support for GCC/Clang? 
	case calling_convention::conv_fastcall: return asmjit::CallConv::kIdHostFastCall;
	case calling_convention::conv_default: return asmjit::CallConv::kIdHost;
	default:
		throw std::runtime_error("Invalid calling convention");
	}
}

void remod::details::asm_code_generator_x86::add_arg_to_dyn_sig(asmjit::FuncSignatureX & sig, std::size_t sizeOfReg)
{
	if (sizeOfReg == 4)
		sig.addArgT<int>();
	else if (sizeOfReg == 2)
		sig.addArgT<short>();
	else if (sizeOfReg == 1)
		sig.addArgT<char>();
	else
		throw std::runtime_error("Other sizes than 4, 2 or 1 bytes are not supported at the moment");
}

// TODO: Validate arg sizes by 
void remod::details::asm_code_generator_x86::add_args_to_dyn_sig(asmjit::FuncSignatureX& sig, const std::vector<std::size_t>& arg_sizes)
{
	for (int i = 0; i < arg_sizes.size(); ++i) {
		add_arg_to_dyn_sig(sig, arg_sizes[i]);
	}
}

/*
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
*/

std::intptr_t remod::details::asm_code_generator_x86::generator_call_conv_detour_2(const detour_point & to_convert, std::intptr_t context_value, std::intptr_t func_to_call)
{
	// Grab the information we need
	calling_convention source_call_conv = to_convert.get_calling_convention();
	source_call_conv = source_call_conv == calling_convention::conv_default ? calling_convention::conv_cdecl : source_call_conv; // default --> cdecl

	const auto& arg_sizes = to_convert.get_arg_sizes();
	
	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	// Init compiler
	asmjit::X86Compiler cc(&code); // Create and attach X86Compiler to `code`.

	// Add Func Wrapper
	std::uint32_t call_conv_id_source = convert_remod_call_conv(source_call_conv);
	asmjit::FuncSignatureX dynCallerFuncSig(call_conv_id_source);
	add_args_to_dyn_sig(dynCallerFuncSig, arg_sizes);
	dynCallerFuncSig.setRetT<int>();
	cc.addFunc(dynCallerFuncSig);

	// Setup target func
	asmjit::FuncSignatureX dynCalleeFuncSig(asmjit::CallConv::kIdHostCDecl);
	dynCalleeFuncSig.addArgT<int>(); // Context value;
	add_args_to_dyn_sig(dynCalleeFuncSig, arg_sizes);
	// Now manage captures
	for (const auto& captures : to_convert.get_captures())
	{
		std::visit(overloaded{
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
				add_arg_to_dyn_sig(dynCalleeFuncSig, register_captures.get_capture_size());
			}
		}, captures);
	}

	dynCalleeFuncSig.setRetT<int>();

	asmjit::CCFuncCall* funcCall = cc.call(func_to_call, dynCalleeFuncSig);
	funcCall->setArg(0, asmjit::Imm(context_value));
	for (int i = 0; i < arg_sizes.size(); ++i) {
		int arg_size = arg_sizes[i];
		if (arg_size == 4) {
			auto reg = cc.newInt32();
			cc.setArg(i, reg);
			funcCall->setArg(i+1, reg);
		} else if (arg_size == 2) {
			auto reg = cc.newInt16();
			cc.setArg(i, reg);
			funcCall->setArg(i+1, reg);
		} else if (arg_size == 1) {
			auto reg = cc.newInt8();
			cc.setArg(i, reg);
			funcCall->setArg(i+1, reg);
		} else
			throw std::runtime_error("Other sizes than 4, 2 or 1 bytes are not supported at the moment");
	}

	int currentArgIndex = arg_sizes.size() + 1;
	// Now manage captures
	for (const auto& captures : to_convert.get_captures())
	{
		std::visit(overloaded{
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
				funcCall->setArg(currentArgIndex++, convert_remod_register(register_captures.get_register()));
			}
		}, captures);
	}
	

	asmjit::X86Gp vReg = cc.newGpd();
	funcCall->setRet(0, vReg);
	cc.ret(vReg);

	cc.endFunc();
	cc.finalize();

	std::intptr_t fn = 0;
	asmjit::ErrorCode err = static_cast<asmjit::ErrorCode>(get_runtime().add(&fn, &code));
	if (err) 
		throw std::runtime_error(std::string("Failed to generate code for detour: ") + asmjit::DebugUtils::errorAsString(err));
	
	return fn;
}

std::intptr_t remod::details::asm_code_generator_x86::generate_orig_func_wrapper(const detour_point& to_convert,
	std::intptr_t func_to_call)
{
	/*

	// Grab the information we need
	calling_convention source_call_conv = to_convert.get_calling_convention();
	source_call_conv = source_call_conv == calling_convention::conv_default ? calling_convention::conv_cdecl : source_call_conv; // default --> cdecl

	const auto& arg_sizes = to_convert.get_arg_sizes();

	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	// Init compiler
	asmjit::X86Compiler cc(&code); // Create and attach X86Compiler to `code`.

	*/

	return 0u;
}

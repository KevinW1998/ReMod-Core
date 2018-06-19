
#include "asm_code_generator_x86.h"


void* remod::details::asm_code_generator_x86::generator_call_conv_detour(detour_point to_convert, void* context_value,
	calling_convention target_call_conv)
{
	// Currently as target only cdecl is supported
	if (target_call_conv != calling_convention::conv_cdecl)
		throw std::runtime_error("Unsupported calling convention");

	
	calling_convention source_call_conv = to_convert.get_calling_convention();

	asmjit::CodeHolder code;// Holds code and relocation information.
	code.init(get_runtime().getCodeInfo());// Initialize to the same arch as JIT runtime.

	asmjit::X86Assembler a(&code);
	// if(source_call_conv == calling_convention::conv_fastcall)
		// a.push()
	
	// Last
	a.push(reinterpret_cast<int>(context_value));
	a.ret();

	void* fn;
	asmjit::Error err = get_runtime().add(&fn, &code);

	return fn;
}

#pragma once

namespace remod
{
	enum class registers : std::uint32_t
	{
		// 8-bit
		ah,
		al,
		ch,
		cl,
		dh,
		dl,
		bh,
		bl,

		// 16-bit
		ax,
		cx,
		dx,
		bx,
		sp,
		bp,
		si,
		di,

		// 32-bit
		eax,
		ecx,
		edx,
		ebx,
		esp,
		ebp,
		esi,
		edi,
		eip,

		// TODO: Only expose if building x64 ?
		// 64-bit
		rax,
		rcx,
		rdx,
		rbx,
		rsp,
		rbp,
		rsi,
		rdi,
		rip
	};

	inline bool is_x64_register(registers reg)
	{
		return reg >= registers::rax;
	}
}



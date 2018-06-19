#pragma once

#include "common.h"
#include "captures.h"

#include <cstdint>
#include <vector>
#include "calling_convention.h"
#include "original_function_call.h"
#include "return_value_source.h"
#include "registers.h"
#include "function_signature.h"

namespace remod
{
	class detour_point
	{
		std::uintptr_t m_offset;
		std::vector<capture_variant_t> m_args_captures;
		std::vector<capture_variant_t> m_extra_captures;
		calling_convention m_convention = calling_convention::conv_stdcall;
		registers m_return_register = registers::eax;
		return_value_source m_ret_val_source = return_value_source::first_detour_function;
		original_function_call m_orig_func_call = original_function_call::after_detours;
		bool m_preserve_whole_stack = true;
	public:
		detour_point(std::uintptr_t offset) : m_offset(offset) {}

		template<typename Ret, typename... Args>
		void init_with_signature(function_signature<Ret(Args...)> sig)
		{
			static_assert(sizeof(Ret) <= sizeof(std::uintptr_t), "Size must be equal or smaller than a processor register size");


		}

		std::uintptr_t get_offset() const
		{
			return m_offset;
		}

		return_value_source get_return_value_source() const
		{
			return m_ret_val_source;
		}

		original_function_call get_original_function_call() const
		{
			return m_orig_func_call;
		}

		calling_convention get_calling_convention() const
		{
			return m_convention;
		}

	};
}



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
#include "details/function_traits.h"

namespace remod
{
	class detour_point
	{
		std::uintptr_t m_offset;
		std::vector<std::size_t> m_args_captures;
		std::vector<capture_variant_t> m_extra_captures;
		calling_convention m_convention = calling_convention::conv_cdecl;
		registers m_return_register = registers::eax;
		return_value_source m_ret_val_source = return_value_source::first_detour_function;
		original_function_call m_orig_func_call = original_function_call::after_detours;
		std::uint32_t m_caller_stack_size = 0;

		template<std::size_t... Ints>
		void init_by_index_seq(std::index_sequence<Ints...>)
		{
			m_args_captures = { Ints... };
		}

	public:
		detour_point(std::uintptr_t offset) : m_offset(offset) {}

		template<typename Ret, typename... Args>
		detour_point(std::uintptr_t offset, function_signature<Ret(Args...)> sig) : m_offset(offset) {
			init_with_signature(sig);
		}

		template<typename Ret, typename... Args>
		detour_point(std::uintptr_t offset, calling_convention conv, function_signature<Ret(Args...)> sig) : m_offset(offset), m_convention(conv) {
			init_with_signature(sig);
		}

		template<typename Ret, typename... Args>
		void init_with_signature(function_signature<Ret(Args...)>)
		{
			using func_type = details::function_traits<Ret(Args...)>;

			static_assert(sizeof(func_type::result_type) <= sizeof(std::uintptr_t), "Size of the return value must be equal or smaller than a processor register size");

			init_by_index_seq(func_type::arg_sizes());
		}

		void add_capture(const capture_variant_t& capture)
		{
			m_extra_captures.push_back(capture);
		}

		void add_capture(capture_variant_t&& capture)
		{
			m_extra_captures.push_back(std::move(capture));
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

		const std::vector<std::size_t>& get_arg_sizes() const
		{
			return m_args_captures;
		}

		void set_convention(calling_convention calling_convention)
		{
			m_convention = calling_convention;
		}

		void set_return_register(registers registers)
		{
			m_return_register = registers;
		}

		void set_ret_val_source(return_value_source return_value_source)
		{
			m_ret_val_source = return_value_source;
		}

		void set_orig_func_call(original_function_call original_function_call)
		{
			m_orig_func_call = original_function_call;
		}

		void set_caller_stack_size(std::uint32_t caller_stack_size)
		{
			m_caller_stack_size = 0;
		}
	};
}



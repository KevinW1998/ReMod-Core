#pragma once

#include "common.h"
#include <variant>
#include "registers.h"

namespace remod
{
	// TODO: Map register with size
	class REMOD_CORE_EXPORT base_capture
	{
		std::size_t m_capture_size;
	public:
		base_capture(std::size_t capture_size);

		std::size_t get_capture_size() const;
	};

	class REMOD_CORE_EXPORT base_offset_capture
	{
		std::uintptr_t m_offset;
	public:
		base_offset_capture(std::uintptr_t offset);

		std::uintptr_t get_offset() const;
	};

	class REMOD_CORE_EXPORT stack_capture : base_offset_capture
	{
	public:
		using base_offset_capture::base_offset_capture;
	};

	class REMOD_CORE_EXPORT register_capture
	{
		registers m_selected_register;
	public:
		register_capture(registers selected_register);

		registers get_register() const;
	};

	class REMOD_CORE_EXPORT argument_capture : base_offset_capture
	{
	public:
		using base_offset_capture::base_offset_capture;
	};

	using capture_variant_t = std::variant<stack_capture, register_capture, argument_capture>;
}



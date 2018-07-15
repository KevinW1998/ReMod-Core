#pragma once

#include "../trackable_patch.h"
#include "../calling_convention.h"
#include "../original_function_call.h"
#include "../return_value_source.h"

namespace remod
{
	class REMOD_CORE_EXPORT trackable_function_patch_base : public trackable_patch
	{
	protected:
		return_value_source m_ret_val_source = return_value_source::first_detour_function;
		original_function_call m_orig_func_call = original_function_call::after_detours;
		calling_convention m_call_conv = calling_convention::conv_default;
		void* m_orig_func = nullptr;
	public:
		// Inherit ctor
		using trackable_patch::trackable_patch;

		void set_return_value_source(return_value_source source);
		void set_original_function_call(original_function_call orig_call);
		void set_original_function(void* orig);
	};
}



#include <remod/details/trackable_function_patch_base.h>


void remod::trackable_function_patch_base::set_return_value_source(return_value_source source)
{
	m_ret_val_source = source;
}

void remod::trackable_function_patch_base::set_original_function_call(original_function_call orig_call)
{
	m_orig_func_call = orig_call;
}

void remod::trackable_function_patch_base::set_original_function(void* orig)
{
	m_orig_func = orig;
}

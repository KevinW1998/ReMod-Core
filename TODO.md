# TODO-List

* Create base_patch_manager<typename resolve_strategy> class
   * impl as typedef
   * function apply<T(...)>(detour_point) : trackable_wrapper<trackable_function_patch<T(...)>>
* Create trackable_patch class
   * patch
   * unpatch
* Create trackable_function_patch<T(...)> class : trackable_patch
   * function add_function
* Create detour_point class with:
   * constructor with single offset
   * stack-capture
   * register-capture
   * argument-capture
   * already pushed arguments
   * select calling convention
   * return-register
   * select return value source [enum]
      * original_function
	  * detour_function
   * call original function [enum]
      * before_detours
      * after_detours
      * no_original_call
* Create global_data container class
* typedefed asm_code_generator
   * Create class base_asm_code_generator?
   * Create class asm_code_generator_x86
   * Create class asm_code_generator_x64
* Setup standases for documentation
* Setup asmjit
* Add template classes for wrapping registers eax<int>, ecx<int>, ect...
* Add template function container: T<Ret(Args...)>


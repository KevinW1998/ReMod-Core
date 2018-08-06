# TODO-List

* Create base_patch_manager<typename resolve_strategy> class
   * impl as typedef  [DONE]
   * function apply<T(...)>(detour_point) : trackable_wrapper<trackable_function_patch<T(...)>> [DONE]
* Create trackable_patch class  [DONE]
   * patch
   * unpatch
* Create trackable_function_patch<T(...)> class : trackable_patch [DONE]
   * function add_function [DONE]
* Create detour_point class with:
   * constructor with single offset [DONE]
   * stack-capture
   * register-capture
   * argument-capture
   * already pushed arguments [DONE]
   * select calling convention [DONE]
   * return-register
   * select return value source [enum] [DONE]
      * original_function
	  * detour_function
   * call original function [enum] [DONE]
      * before_detours
      * after_detours
      * no_original_call
* Implement custom calling conventions
* Create trackable_light_function_patch<T(...)> class --> uses global storage for lambda
* Create global_data container class [DONE]
* typedefed asm_code_generator 
   * Create class base_asm_code_generator?
   * Create class asm_code_generator_x86 [DONE]
   * Create class asm_code_generator_x64
* Setup standases for documentation
* Setup asmjit [DONE]
* Add template classes for wrapping registers eax<int>, ecx<int>, ect...
* Add template function container: T<Ret(Args...)>


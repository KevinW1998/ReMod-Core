# TODO-List

* Create base_patch_manager<typename resolve_strategy, typename impl> class
   * function apply(detour_point)
   * function apply_with_tracker(detour_point) : trackable_patch
* Create detour_point class with:
   * constructor with single offset
   * stack-capture
   * register-capture
   * argument-capture
   * already pushed arguments
   * attach std::function (or in already patched one? or both?)
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

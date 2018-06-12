# TODO-List

* Create base_patch_manager<typename resolve_strategy, typename impl> class
   * function apply(detour_point)
   * function apply_with_tracker(detour_point) : trackable_patch
* Create detour_point class with:
   * constructor with single offset
   * stack-capture
   * register-capture
   * argument-capture
   * attach std::function (or in already patched one? or both?)
   * select calling convention
   * return-register
   * call original function
      * before_detours
      * after_detours
      * no_original_call
* Create global_data container class

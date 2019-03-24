#pragma once

#include "common.h"
#include "base_patch_manager.h"
#include "module.h"
#include "trackable_wrapper.h"
#include "trackable_function_patch.h"
#include "detour_point.h"
#include "function_signature.h"
#include "details/function_traits.h"
#include "trackable_memory_patch.h"

namespace remod
{
	template<typename ResolveStrategy>
	class patch_manager : base_patch_manager
	{
		module m_mod;

		template<typename Ret, typename... Args>
		trackable_wrapper<trackable_function_patch<Ret(Args...)>> apply_impl(const detour_point& detour_point_to_apply, details::function_traits<Ret(Args...)>)
		{
			
			static_assert(sizeof(Ret) <= 4, "Currently only return values less than or equal 4 bytes are supported!");

			// Resolve address
			std::uintptr_t addr = detour_point_to_apply.get_offset();
			ResolveStrategy::resolve(addr, m_mod);

			trackable_function_patch<Ret(Args...)>* func_patch = new trackable_function_patch<Ret(Args...)>;
			
			get_patch_store()->add_patch(func_patch);
			func_patch->set_original_function_call(detour_point_to_apply.get_original_function_call());
			func_patch->set_return_value_source(detour_point_to_apply.get_return_value_source());
			
			auto proxy = +[](trackable_function_patch<Ret(Args...)>* patch, Args... args)
			{
				return (*patch)(args...);
			};			
			
			// Generate the on_patch and apply the on_patch
			generate_function_patch(addr, func_patch, reinterpret_cast<void*>(proxy), detour_point_to_apply);
			func_patch->patch();
			
			return { get_patch_store(), func_patch };
		}
	public:
		patch_manager(const module& mod = remod::main_module) noexcept : m_mod(mod)
		{}

		template<typename Ret, typename... Args>
		auto apply_trackable_function_patch(const detour_point& detour_point_to_apply, function_signature<Ret(Args...)>)
		{
			return apply_impl(detour_point_to_apply, details::function_traits<Ret(Args...)>{});
		}

		template<typename Func>
		auto apply_trackable_function_patch(const detour_point& detour_point_to_apply, Func func)
		{
			auto ret = apply_trackable_function_patch(detour_point_to_apply, signature_from_function(func));
			ret->add_detour_function(func);
			return ret;
		}

		trackable_wrapper<trackable_memory_patch> create_trackable_memory_patch()
		{
			trackable_memory_patch* mem_patch = new trackable_memory_patch;
			
			details::patch_store* patch_store = get_patch_store();
			patch_store->add_patch(mem_patch);
			return { patch_store, mem_patch };
		}

		template<typename T>
		trackable_wrapper<trackable_memory_patch> create_trackable_memory_patch(std::uintptr_t pointer, T value)
		{
			static_assert(std::is_pod_v<T>, "Only pod types are supported for memory patches");

			trackable_wrapper<trackable_memory_patch> wrapper = create_trackable_memory_patch();
			wrapper->set_patch_pointer(pointer);
			wrapper->set_as_binary_data(value);

			return wrapper;
		}

		trackable_wrapper<trackable_memory_patch> create_trackable_memory_patch_with_data(std::uintptr_t pointer, std::vector<std::uint8_t> data)
		{
			trackable_wrapper<trackable_memory_patch> wrapper = create_trackable_memory_patch();
			wrapper->set_patch_pointer(pointer);
			wrapper->set_binary_data(std::move(data));

			return wrapper;
		}

		template<typename T>
		void write_memory(std::uintptr_t offset, T value) {
			static_assert(std::is_pod_v<T>, "Only pod types are supported for memory writes");

			ResolveStrategy::resolve(offset, m_mod);

			write_memory_data(offset, &value, sizeof(T));
		}

		template<typename T>
		T read_memory(std::uintptr_t offset) {
			static_assert(std::is_pod_v<T>, "Only pod types are supported for memory reads");

			ResolveStrategy::resolve(offset, m_mod);

			T out;
			read_memory_data(offset, &out, sizeof(T));
			return out;
		}

		std::uintptr_t resolve(std::uintptr_t offset) {
			ResolveStrategy::resolve(offset, m_mod);
			return offset;
		}
	};
}





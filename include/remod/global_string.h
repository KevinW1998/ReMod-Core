#pragma once

#include <remod/global_pointer.h>
#include <type_traits>
#include <xstring>

namespace remod
{
	template<typename CharT, typename ResolveStrategy>
	class global_basic_string : public global_pointer<ResolveStrategy>
	{
	public:
		using global_pointer<ResolveStrategy>::global_pointer;

		static_assert(std::is_pod_v<CharT>, "CharT must be pod value");

		operator std::basic_string_view<CharT>() {
			return get_view();
		}

		std::basic_string_view<CharT> get_view()
		{
			return std::basic_string_view<CharT>(reinterpret_cast<CharT*>(get_pointer()));
		}

	};

	template<typename ResolveStrategy>
	using global_string = global_basic_string<char, ResolveStrategy>;

	template<typename ResolveStrategy>
	using global_wstring = global_basic_string<wchar_t, ResolveStrategy>;
}


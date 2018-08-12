#include <catch.hpp>
#include "remod/remod_definitions.h"
#include "remod.h"

const char test_str1[] = "Hello World";
const wchar_t test_str2[] = L"Hello Wide World!";

remod::global_string_no_resolve test_str1_view{ reinterpret_cast<std::uintptr_t>(test_str1) };
remod::global_wstring_no_resolve test_str2_view{ reinterpret_cast<std::uintptr_t>(test_str2) };


TEST_CASE("Test global string without resolve", "[remod-global-string]")
{
	remod::resolve_all();

	// Value check
	REQUIRE(test_str1_view.get_view() == "Hello World");
	REQUIRE(test_str2_view.get_view() == L"Hello Wide World!");

	// operator check
	std::string_view tmp_str1_view = test_str1_view;
	std::wstring_view tmp_str2_view = test_str2_view;

	// Test if valid
	REQUIRE(tmp_str1_view[0] == 'H');
	REQUIRE(tmp_str2_view[7] == L'i');


}


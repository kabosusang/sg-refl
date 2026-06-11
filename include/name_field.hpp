#pragma once
#include <source_location>
#include <string_view>

#include "string_literal.hpp"

namespace reflect {
template <auto ptr>
consteval std::string_view member_name() {
#if defined(__clang__) || defined(__GNUC__)
	const auto sig = std::string_view{ __PRETTY_FUNCTION__ };
#elif defined(_MSC_VER)
	const auto sig = std::string_view{ __FUNCSIG__ };
#else
	const auto sig = std::string_view{
		std::source_location::current().function_name()
	};
#endif

	auto amp_pos = sig.rfind('&');
	if (amp_pos != sig.npos) {
		auto start = amp_pos + 1;

		auto dot_pos = sig.rfind('.');
		auto colon_pos = sig.rfind("::");

		if (colon_pos != sig.npos && colon_pos > amp_pos) {
			start = colon_pos + 2;
		} else if (dot_pos != sig.npos && dot_pos > amp_pos) {
			start = dot_pos + 1;
		}

		auto end = sig.find(']', start);
		if (end == sig.npos) {
			end = sig.size();
		}
		return sig.substr(start, end - start);
	}

	return "";
}

template <auto ptr>
consteval auto member_name_lit() {
	constexpr auto name = member_name<ptr>();
	constexpr auto N = name.size();

	string_literal<N + 1> result;
	for (std::size_t i = 0; i < N; ++i) {
		result.data_[i] = name[i];
	}
	return result;
}

} //namespace reflect

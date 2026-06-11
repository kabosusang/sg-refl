#pragma once

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace reflect {
template <size_t N>
	requires(N > 0)
struct string_literal {
	std::array<char, N> data_{};

	constexpr string_literal() = default;
	constexpr string_literal(const char (&str)[N]) {
		std::copy_n(str, N, data_.begin());
	}

	[[nodiscard("ignore return value to string_literal->sv()")]]
	constexpr auto sv() const noexcept {
		return std::string_view(data_.data(), N - 1);
	}

	static constexpr size_t size = N - 1;
};
} //namespace reflect

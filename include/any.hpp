#pragma once
#include <cstddef>

namespace reflect {

struct any{
	constexpr any(std::size_t) {}
	template <typename U>
	constexpr operator U() const noexcept { return {}; }
};

} //namespace reflect

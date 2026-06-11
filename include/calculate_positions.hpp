#pragma once
#include <array>
#include <cstddef>

namespace reflect {

constexpr unsigned int align(unsigned int offset, unsigned int alignment) {
	return (offset + alignment - 1) & ~(alignment - 1);
}

template <class... Types>
consteval auto calculate_positions() {
	std::array<unsigned int, sizeof...(Types) + 1> result{};
	unsigned int current = 0;
	std::size_t i = 0;

	((current = align(current, alignof(Types)),
			 result[i++] = current,
			 current += sizeof(Types)),
			...);
	result[i] = current;
	return result;
}

} //namespace reflect

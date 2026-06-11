#pragma once
#include <tuple>

#include "count_fields.hpp"
namespace reflect {

template <typename T, std::size_t N>
struct bind_to_tuple_impl;

#define MAKE_BIND(N, ...)                 \
	template <typename T>                 \
	struct bind_to_tuple_impl<T, N> {     \
		static auto apply(T& t) {         \
			auto& [__VA_ARGS__] = t;      \
			return std::tie(__VA_ARGS__); \
		}                                 \
	};

MAKE_BIND(1, f1)
MAKE_BIND(2, f1, f2)
MAKE_BIND(3, f1, f2, f3)
MAKE_BIND(4, f1, f2, f3, f4)
MAKE_BIND(5, f1, f2, f3, f4, f5)
MAKE_BIND(6, f1, f2, f3, f4, f5, f6)
MAKE_BIND(7, f1, f2, f3, f4, f5, f6, f7)
MAKE_BIND(8, f1, f2, f3, f4, f5, f6, f7, f8)

#undef MAKE_BIND

template <typename T>
[[nodiscard]]
constexpr auto bind_to_tuple(T& t) {
	return bind_to_tuple_impl<T, count_fields_v<T>>::apply(t);
}

template <typename T, typename F>
[[nodiscard]]
constexpr auto bind_to_tuple(T& t, const F& _f) {
	auto view = bind_to_tuple(t);
	return [&]<std::size_t... _is>(std::index_sequence<_is...>) {
		return std::make_tuple(_f(&std::get<_is>(view))...);
	}(std::make_index_sequence<count_fields_v<T>>());
}

} //namespace reflect

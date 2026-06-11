#pragma once
#include <utility>
#include <type_traits>

namespace reflect {

template <class T, int N, int _i>
struct element_wrapper {
	T t_;
};

template <int N, class T1, int _i1, class T2, int _i2>
constexpr auto operator|(const element_wrapper<T1, N, _i1>& _e1,
		const element_wrapper<T2, N, _i2>& _e2) {
	if constexpr (_i1 == N) {
		return _e1;
	} else {
		return _e2;
	}
}

template <class Head, class... Tail>
constexpr auto find_matching_element(const Head& _head, const Tail&... _tail) {
	return (_head | ... | _tail);
}

template <int N, class... Ts, int... _is>
constexpr auto wrap_elements(Ts... _ts, std::integer_sequence<int, _is...>) {
	return find_matching_element(element_wrapper<Ts, N, _is>{ _ts }...).t_;
}

template <int N, class... Ts>
constexpr auto nth_element(Ts... _ts) {
	static_assert(N >= 0, "N out of bounds.");
	static_assert(N < sizeof...(Ts), "N out of bounds.");
	return wrap_elements<N, Ts...>(
			_ts..., std::make_integer_sequence<int, sizeof...(Ts)>());
}

template <class T>
struct TypeWrapper {
	using Type = T;
};


template <int N, class... Ts>
using nth_element_t =
		typename std::invoke_result_t<decltype(nth_element<N, TypeWrapper<Ts>...>),
				TypeWrapper<Ts>...>::Type;

} //namespace reflect

#pragma once
#include <tuple>
#include <utility>

#include "field.hpp"

namespace reflect {
template <typename... Fields>
struct named_tuple {
	std::tuple<typename Fields::Type...> values;

	constexpr named_tuple() = default;
	constexpr named_tuple(typename Fields::Type... vals)
		requires(sizeof...(Fields) > 0)
			: values(std::move(vals)...) {}

	template <string_literal name>
	[[nodiscard("ignore return value to NamedTuple->get()")]]
	constexpr decltype(auto) get() {
		return get_impl<name>(std::make_index_sequence<sizeof...(Fields)>{});
	}

	template <string_literal name>
	[[nodiscard("ignore return value to NamedTuple->get()")]]
	constexpr decltype(auto) get() const {
		return get_impl<name>(std::make_index_sequence<sizeof...(Fields)>{});
	}

	template <std::size_t I>
	[[nodiscard("ignore return value to NamedTuple->get()")]]
	constexpr decltype(auto) get() { return std::get<I>(values); }

	template <std::size_t I>
	[[nodiscard("ignore return value to NamedTuple->get()")]]
	constexpr decltype(auto) get() const { return std::get<I>(values); }

	static constexpr std::size_t size = sizeof...(Fields);

private:
	template <string_literal name, std::size_t... Is>
	constexpr decltype(auto) get_impl(std::index_sequence<Is...>) {
		constexpr auto result = []() -> std::pair<std::size_t, bool> {
			std::size_t i = 0;
			bool f = false;
			((!f && Fields::name.sv() == name.sv() ? (f = true, i = Is) : 0), ...);
			return { i, f };
		}();

		static_assert(result.second, "Field name not found in named_tuple.");

		return get_impl_by_idx<result.first, Is...>();
	}

	template <std::size_t TargetIdx, std::size_t I, std::size_t... Is>
	constexpr decltype(auto) get_impl_by_idx() {
		if constexpr (TargetIdx == I) {
			return std::get<I>(values);
		} else if constexpr (sizeof...(Is) > 0) {
			return get_impl_by_idx<TargetIdx, Is...>();
		} else {
			return std::get<0>(values); // fallback，不应到达
		}
	}
};

// template <string_literal n1, typename T1, string_literal n2, typename T2>
// constexpr auto operator*(field<n1, T1> f1, field<n2, T2> f2) {
// 	return named_tuple<field<n1, T1>, field<n2, T2>>(f1.value, f2.value);
// }

// template <string_literal n, typename T, typename... Fs>
// constexpr auto operator*(named_tuple<Fs...> nt, field<n, T> f) {
// 	return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
// 		return named_tuple<Fs..., field<n, T>>(
// 				std::get<Is>(nt.values)...,
// 				f.value);
// 	}(std::make_index_sequence<sizeof...(Fs)>{});
// }

// template <string_literal n, typename T, typename... Fs>
// constexpr auto operator*(field<n, T> f, named_tuple<Fs...> nt) {
// 	return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
// 		return named_tuple<field<n, T>, Fs...>(
// 				f.value,
// 				std::get<Is>(nt.values)...);
// 	}(std::make_index_sequence<sizeof...(Fs)>{});
// }

template <string_literal name, typename NamedTuple>
	requires requires(NamedTuple nt) { nt.template get<name>(); }
constexpr decltype(auto) get(NamedTuple&& nt) {
	return std::forward<NamedTuple>(nt).template get<name>();
}
} //namespace reflect

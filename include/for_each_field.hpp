#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include "bind_to_tuple.hpp"
#include "fake_object_field.hpp"
#include "has_fields.hpp"

namespace reflect {

template <typename FieldType>
constexpr auto get_name_from_field() {
	return FieldType::name;
}

template <typename T, std::size_t I>
constexpr auto get_name_non_intrusive() {
	constexpr auto ptr = get_ith_field_ptr<T, I>();
	return member_name_lit<ptr>();
}

template <auto V>
struct field_name_wrapper {
	static constexpr auto value = V;
	constexpr auto sv() const { return V.sv(); }
	constexpr auto data() const { return V.data(); }
	constexpr auto size() const { return V.size(); }

	constexpr bool operator==(const decltype(V)& other) const {
		return V == other;
	}

	constexpr operator decltype(V)() const { return V; }
};

template <typename T, typename F>
constexpr void for_each_field(T&& obj, F&& f) {
	using RawT = std::decay_t<T>;
	auto&& refs = bind_to_tuple(std::forward<T>(obj));

	[&]<std::size_t... Is>(std::index_sequence<Is...>) {
		if constexpr (has_fields_v<RawT>) {
			(f(
					 field_name_wrapper<std::remove_reference_t<decltype(std::get<Is>(refs))>::name>{},
					 std::get<Is>(refs).value),
					...);
		} else {
			// 非侵入式
			(f(
					 field_name_wrapper<get_name_non_intrusive<RawT, Is>()>{},
					 std::get<Is>(refs)),
					...);
		}
	}(std::make_index_sequence<count_fields_v<RawT>>());
}

} //namespace reflect

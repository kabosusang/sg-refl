#pragma once
#include <utility>

#include "bind_to_tuple.hpp"
#include "is_field.hpp"
namespace reflect {

template <typename T>
constexpr bool has_fields_v = [] {
	if constexpr (count_fields_v<T> == 0) {
		return false;
	} else {
		constexpr auto N = count_fields_v<T>;
		using return_tuple = decltype(bind_to_tuple_impl<T, N>::apply(std::declval<T&>()));
		using first_number = std::tuple_element_t<0, return_tuple>;

		return is_field_v<first_number>;
	}
}();

} //namespace reflect

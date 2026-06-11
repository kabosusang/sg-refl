#pragma once

#include "for_each_field.hpp"
#include "to_named_tuple.hpp"

#define FIELD_NAME_IS(field_name, str) (std::decay_t<decltype(field_name)>::value.sv() == str)


namespace reflect {

template <typename T>
constexpr decltype(auto) bind_named_tuple(T&& obj) {
	return to_named_tuple(std::forward<T>(obj));
}

template <typename T, typename F>
constexpr void for_each(T&& obj, F&& f) {
	for_each_field(std::forward<T>(obj), std::forward<F>(f));
}



} //namespace reflect

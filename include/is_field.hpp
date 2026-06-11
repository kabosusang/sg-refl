#pragma once
#include "field.hpp"

namespace reflect {

template <class T>
struct is_field : std::false_type {};

template <string_literal name, class Type>
struct is_field<field<name, Type>> : std::true_type {};

template <class T>
constexpr bool is_field_v = is_field<std::remove_cvref_t<T>>::value;

}













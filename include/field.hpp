#pragma once
#include "string_literal.hpp"

namespace reflect {
template <string_literal name_, typename T>
struct field {
	using Type = T;

	static constexpr auto name = name_;
	T value;

	constexpr field() : value{} {}
	constexpr field(const T& v) : value(v) {}
	constexpr field(T&& v) : value(std::move(v)) {}

	constexpr T& operator*() { return value; }
	constexpr const T& operator*() const { return value; }

	constexpr T* operator->() { return &value; }
	constexpr const T* operator->() const { return &value; }
};
} //namespace reflect

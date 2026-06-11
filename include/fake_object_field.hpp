#pragma once

#include "count_fields.hpp"
#include "nth_element_t.hpp"

namespace reflect {

template <typename T>
struct fake_object {
	static constexpr T obj{};
};

template <typename T>
consteval const T& get_fake_object() {
	return fake_object<T>::obj;
}

template <typename T, std::size_t N>
struct fake_object_helper;

#define FAKE_OBJ(N, ...)                                      \
	template <typename T>                                     \
	struct fake_object_helper<T, N> {                         \
		template <int _i>                                     \
		static consteval auto get_field() {                   \
			const auto& [__VA_ARGS__] = get_fake_object<T>(); \
			return [&](const auto&... refs) {                 \
				return nth_element<_i>(&refs...);             \
			}(__VA_ARGS__);                                   \
		}                                                     \
	};

FAKE_OBJ(1, f0)
FAKE_OBJ(2, f0, f1)
FAKE_OBJ(3, f0, f1, f2)
FAKE_OBJ(4, f0, f1, f2, f3)
FAKE_OBJ(5, f0, f1, f2, f3, f4)
FAKE_OBJ(6, f0, f1, f2, f3, f4, f5)
FAKE_OBJ(7, f0, f1, f2, f3, f4, f5, f6)
FAKE_OBJ(8, f0, f1, f2, f3, f4, f5, f6, f7)

#undef FAKE_OBJ

template <typename T, std::size_t i>
consteval auto get_ith_field_ptr() {
	return fake_object_helper<T, count_fields_v<T>>::template get_field<i>();
}

} //namespace reflect

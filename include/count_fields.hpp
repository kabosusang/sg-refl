#pragma once

//include
#include <cstddef>
#include <type_traits>
#include <utility>

#include "any.hpp"
namespace reflect {

template <typename T>
struct count_fields_impl {
	template <std::size_t N>
	static consteval bool constructible() {
		return []<std::size_t... Is>(std::index_sequence<Is...>) {
			return requires { T{ any(Is)... }; };
		}(std::make_index_sequence<N>());
	}

	template <std::size_t N = 0>
	static consteval std::size_t count() {
        if constexpr (N > 16) {
        return 16;             
    }else if constexpr (!constructible<N>()) {
			return N - 1;
		} else {
			return count<N + 1>();
		}
	}
};

template <typename T>
struct count_fields {
	static constexpr std::size_t value = count_fields_impl<T>::count();
};

template <typename T>
constexpr std::size_t count_fields_v = count_fields<T>::value;
} //namespace reflect

#pragma once
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "bind_to_tuple.hpp"
#include "fake_object_field.hpp"
#include "field.hpp"
#include "has_fields.hpp"
#include "name_field.hpp"
#include "name_tuple.hpp"

namespace reflect {
template <typename T>
auto wrap_in_fields(T&& obj) {
	constexpr auto N = count_fields_v<std::decay_t<T>>;

	if constexpr (N == 0) {
		return std::make_tuple();
	} else {
		auto&& refs = bind_to_tuple(obj);

		return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
			return std::make_tuple(
					field<member_name_lit<get_ith_field_ptr<std::decay_t<T>, Is>()>(), std::decay_t<std::tuple_element_t<Is, std::decay_t<decltype(refs)>>>>(std::tuple_element_t<Is, std::decay_t<decltype(refs)>>(std::get<Is>(refs)))...);
		}(std::make_index_sequence<N>());
	}
}

template <typename T>
auto intrusive_to_fields(T&& obj) {
	using RawT = std::decay_t<T>;
	auto&& refs = bind_to_tuple(std::forward<T>(obj));
	using RefTuple = std::decay_t<decltype(refs)>;
	constexpr auto N = count_fields_v<RawT>;

	return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
		return std::make_tuple(
				field<std::remove_reference_t<std::tuple_element_t<Is, RefTuple>>::name, typename std::remove_reference_t<std::tuple_element_t<Is, RefTuple>>::Type>(std::forward<decltype(std::get<Is>(refs).value)>(std::get<Is>(refs).value))...);
	}(std::make_index_sequence<N>());
}

// 主函数
template <typename T>
auto to_named_tuple(T&& obj) {
	using RawT = std::decay_t<T>;
	constexpr auto N = count_fields_v<RawT>;

	if constexpr (N == 0) {
		return named_tuple<>{};
	} else if constexpr (has_fields_v<RawT>) {
		//别名路径
		auto fields = intrusive_to_fields(std::forward<T>(obj));

		return std::apply([](auto&&... fs) {
			return named_tuple<std::decay_t<decltype(fs)>...>(
					std::forward<decltype(fs.value)>(fs.value)...);
		},
				std::move(fields));
	} else {
		// 无别名路径
		auto fields = wrap_in_fields(std::forward<T>(obj));

		return std::apply([](auto&&... fs) {
			return named_tuple<std::decay_t<decltype(fs)>...>(
					std::forward<decltype(fs.value)>(fs.value)...);
		},
				std::move(fields));
	}
}

} //namespace reflect

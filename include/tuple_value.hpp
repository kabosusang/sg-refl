#pragma once

#include <array>
#include <utility>

#include "calculate_positions.hpp"
#include "nth_element_t.hpp"
#include "ptr_case.hpp"

namespace reflect {

template <class... FieldTypes>
class tuple;

template <>
class tuple<> {
public:
	tuple() {}
};

template <class... Types>
class tuple {
	static constexpr std::size_t size_ = sizeof...(Types);

	static constexpr auto positions_ = calculate_positions<Types...>();

	//cache
	static constexpr auto seq_ = std::make_integer_sequence<int, size_>{};

	//all size
	static constexpr unsigned int num_bytes_ = std::get<size_>(positions_);

	using DataType = std::array<unsigned char, num_bytes_>;

	//raw
	alignas(Types...) DataType data_;

	template <int _i>
	static consteval unsigned int pos() {
		return std::get<_i>(positions_);
	}

	//===========================construct func =========================
private:
	template <int... _is>
	void copy_from_other(const tuple& _other,
			std::integer_sequence<int, _is...>) {
		const auto copy_one = [this]<int _i>(const auto& _other,
									  std::integral_constant<int, _i>) {
			using Type = nth_element_t<_i, Types...>;
			::new (static_cast<void*>(data_.data() + pos<_i>()))
					Type(_other.template get<_i>());
		};
		(copy_one(_other, std::integral_constant<int, _is>{}), ...);
	}

	template <int... _is>
	void copy_from_types(const Types&... _types,
			std::integer_sequence<int, _is...>) {
		const auto copy_one = [this]<int _i>(const auto& _t,
									  std::integral_constant<int, _i>) {
			using Type = nth_element_t<_i, Types...>;
			::new (static_cast<void*>(data_.data() + pos<_i>())) Type(_t);
		};
		(copy_one(_types, std::integral_constant<int, _is>{}), ...);
	}

	template <int... _is>
	void destroy_if_necessary(std::integer_sequence<int, _is...>) {
		const auto destroy_one = [](auto& _t) {
			using Type = std::remove_cvref_t<decltype(_t)>;
			if constexpr (std::is_destructible_v<Type>) {
				_t.~Type();
			}
		};
		(destroy_one(get<_is>()), ...);
	}

	template <int... _is>
	void move_from_other(tuple&& _other, std::integer_sequence<int, _is...>) {
		const auto move_one = [this]<int _i>(auto&& _other,
									  std::integral_constant<int, _i>) {
			using Type = nth_element_t<_i, Types...>;
			::new (static_cast<void*>(data_.data() + pos<_i>()))
					Type(std::move(_other.template get<_i>()));
		};
		(move_one(_other, std::integral_constant<int, _is>{}), ...);
	}

	template <int... _is>
	void move_from_types(Types&&... _types, std::integer_sequence<int, _is...>) {
		const auto move_one = [this]<int _i>(auto&& _t,
									  std::integral_constant<int, _i>) {
			using Type = nth_element_t<_i, Types...>;
			::new (static_cast<void*>(data_.data() + pos<_i>())) Type(std::move(_t));
		};
		(move_one(std::move(_types), std::integral_constant<int, _is>{}), ...);
	}

public:
	//construct
	tuple(const Types&... _t) { copy_from_types(_t..., seq_); }

	tuple(Types&&... _t) noexcept { move_from_types(std::move(_t)..., seq_); }

	tuple() : tuple(Types()...) {}

	tuple(const tuple& _other) { copy_from_other(_other, seq_); }

	tuple(tuple&& _other) noexcept { move_from_other(std::move(_other), seq_); }

	~tuple() { destroy_if_necessary(seq_); }

	template <std::size_t _index>
	constexpr auto& get() {
		using Type = nth_element_t<_index, Types...>;
		return *ptr_cast<Type*>(data_.data() + pos<_index>());
	}

	template <int _index>
	constexpr const auto& get() const {
		using Type = nth_element_t<_index, Types...>;
		return *ptr_cast<const Type*>(data_.data() + pos<_index>());
	}
};

// ===================== inline func ====================================

template <int _index, class... Types>
inline constexpr auto& get(tuple<Types...>& _tup) {
	return _tup.template get<_index>();
}

template <class... Types>
inline constexpr auto make_tuple(Types&&... _args) {
	return tuple<std::decay_t<Types>...>(std::forward<Types>(_args)...);
}

template <class T>
struct tuple_size {
	static_assert("error tuple_size instance!");
};

template <class... Ts>
struct tuple_size<tuple<Ts...>> {
	static constexpr auto value = sizeof...(Ts);
};

} //namespace reflect

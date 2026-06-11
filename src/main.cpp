#include <cassert>
#include <iostream>
#include <vector>

#include "sgrefl.hpp"

struct Health {
    int hp;
    int magic;
};

struct Point {
	double x, y;
};
struct Player {
	reflect::field<"name", std::string> name_;
	reflect::field<"hp", int> hp_;
	reflect::field<"level", int> level_;
};
struct Polygon {
	double x, y;
	std::vector<Point> vertices;
	std::vector<int> indices;
};

struct test_fied_array {
	std::array<int, 3> tf_;
	int tf2;
};

struct test_fied_c_array {
	int tf_[3];
	int tf2;
}; //error count_fields_v == 4

int main() {
	//
	static_assert(reflect::count_fields_v<test_fied_array> == 2);
	Health hl{ 100, 55 };
	auto h = reflect::bind_named_tuple(hl);

	std::cout << "----------- 基础打印 -----------" << std::endl;
	std::cout << get<"hp">(h) << std::endl;
	std::cout << get<"magic">(h) << std::endl;

	Player player{ { "link" }, 100, 10 };

	std::cout << "----------- for_each -----------" << std::endl;
	std::cout << "----------- player :修改前 --------" << std::endl;
	//修改前
	reflect::for_each(player, [](const auto& name, auto& value) {
		if constexpr (FIELD_NAME_IS(name, "name")) {
			std::cout << "player name: " << value << std::endl;
			value = { "lara" };
		} else if constexpr (FIELD_NAME_IS(name, "hp")) {
			std::cout << "player hp: " << value << std::endl;
			value = 80;
		} else if constexpr (FIELD_NAME_IS(name, "level")) {
			std::cout << "player level: " << value << std::endl;
			value = 55;
		}
	});

	//修改后
	std::cout << "----------- player :修改后 --------" << std::endl;
	reflect::for_each(player, [&](auto&& name, auto&& value) {
		std::cout << "player : " << name.sv() << "-- " << value << std::endl;
	});

	std::cout << "----------- storage -----------" << std::endl;

	Polygon poly{
		1.0, 2.0,
		{ { 0, 0 }, { 1, 1 }, { 2, 2 } },
		{ 0, 1, 2 }
	};






	
}

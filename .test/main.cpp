// 
// Copyright 2021 Clemens Cords
// Created on 17.12.21 by clem (mail@clemens-cords.com)
//

#include <iostream>
#include <jluna.hpp>
#include <functional>
#include <proxy.hpp>
#include <global_utilities.hpp>
#include <array_proxy.hpp>

using namespace jluna;

int main()
{
    // initialize state, always needs to be called first
    State::initialize();

    State::safe_script(R"(
        arr = Array{UInt64, 3}(reshape(collect(1:(3*3*3)), 3, 3, 3))
        vec = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    )");

    jluna::Vector<size_t> vec = Main["vec"];

    vec.push_front(99);
    vec.push_back(88);
    vec.insert(5, 123);
    vec.erase(2);

    for (auto i : vec)
        std::cout << i.operator int() << std::endl;

    /*

    // running code:
    State::safe_script("println(\"hello jluna\")"); // hello jluna

    // accessing variables by value: use State::safe_script()
    auto value_123 = State::safe_script("return 123");
    int as_int = value_123;

    std::cout << as_int << std::endl; // 123

    // accessing variables by reference: use operator[]
    State::safe_script("julia_value_456 = 456");
    auto value_456 = Main["julia_value_456"];
    std::cout << ((int) value_456) << std::endl; // 456
    make_mutating(value_456);
    value_456 = 789; // this modifies the variable in main
    State::safe_script("println(Main.julia_value_456)"); // prints: 789

    State::safe_script(R"(
        mutable struct StructType
            _field

            StructType() = new(undef)
        end

        struct_type_instance = StructType();
    )");

    auto field = Main["struct_type_instance"]["_field"];
    make_mutating(field);
    field = 101112;
    forward_last_exception();
    State::safe_script("println(struct_type_instance._field)"); // prints: 101112
     */

}
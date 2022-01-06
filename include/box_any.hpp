// 
// Copyright 2021 Clemens Cords
// Created on 24.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>
#include <type_traits>
#include <string>

namespace jluna
{
    // identity
    jl_value_t* box(jl_value_t* value)
    {
        return value;
    }

    template<Decayable T>
    jl_value_t* box(T&& value)
    {
        return (jl_value_t*) (std::forward<T*>(value));
    }

    // string
    jl_value_t* box(const std::string& value)
    {
        std::string command = "return \"" + value + "\"";
        return jl_eval_string(command.c_str());
    }

    // c string
    jl_value_t* box(const char* value)
    {
        std::string command = "return \"" + std::string(value) + "\"";
        return jl_eval_string(command.c_str());
    }

    // bool
    jl_value_t* box(bool value)
    {
        return jl_box_bool(value);
    }

    // char
    jl_value_t* box(char value)
    {
        return jl_box_char(value);
    }

    // int8
    jl_value_t* box(int8_t value)
    {
        return jl_box_int8(value);
    }

    // int16
    jl_value_t* box(int16_t value)
    {
        return jl_box_int16(value);
    }

    // int32
    jl_value_t* box(int32_t value)
    {
        return jl_box_int32(value);
    }

    // int64
    jl_value_t* box(int64_t value)
    {
        return jl_box_int64(value);
    }

    // uint8
    jl_value_t* box(uint8_t value)
    {
        return jl_box_uint8(value);
    }

    // uint16
    jl_value_t* box(uint16_t value)
    {
        return jl_box_uint16(value);
    }

    // uint32
    jl_value_t* box(uint32_t value)
    {
        return jl_box_uint32(value);
    }

    // uint64
    jl_value_t* box(uint64_t value)
    {
        return jl_box_uint64(value);
    }

    // float
    jl_value_t* box(float value)
    {
        return jl_box_float32(value);
    }

    // double
    jl_value_t* box(double value)
    {
        return jl_box_float64(value);
    }

    // explicit return type
    template<typename From, typename To>
    concept CastableTo = requires(From t)
    {
        {static_cast<To>(t)};
    };

    template<typename Return_t, CastableTo<Return_t> Arg_t>
    jl_value_t* box(Arg_t t)
    {
        return box(static_cast<Return_t>(t));
    }
}
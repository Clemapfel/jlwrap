// 
// Copyright 2021 Clemens Cords
// Created on 19.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <proxy.hpp>

namespace jlwrap
{
    union State;

    class Function : protected Proxy<State>
    {
        public:
            Function() = delete;

            /// @brief call operator
            /// @tparams Args_t: argument types
            /// @param args
            /// @returns determined by context
            template<typename... Args_t>
            decltype(auto) operator()(Args_t...);

        protected:
            Function(jl_function_t*);

        private:
            jl_function_t* _value;
    };
}

#include ".src/function_proxy.inl"
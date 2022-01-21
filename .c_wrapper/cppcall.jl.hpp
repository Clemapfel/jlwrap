#pragma once

namespace jluna::c_adapter
{
    const char* cppcall_module = R"(
"""
helper module used by jluna c_adapter and cppcall
"""
module _cppcall

    mutable struct State
        _arguments::Tuple
        _result::Any

        State() = new((), nothing)
    end

    const _library_name = "./libjluna_c_adapter.so"
    _state = Base.Ref{_cppcall.State}(State())

    """
    an exception thrown when trying to invoke cppcall with a function name that
    has not yet been registered via jluna::register_function
    """
    mutable struct UnregisteredFunctionNameException <: Exception

        _function_name::Symbol
    end
    Base.showerror(io::IO, e::UnregisteredFunctionNameException) = print(io, "UnregisteredFunctionNameException: no C++ function with name :" * string(e._function_name) * " registered")

    """
    `set_result(::Any) -> Nothing`
    
    modify _cppcall state result
    """
    function set_result(x::Any) ::Nothing

        global _cppcall._state[]._result = x
        return nothing
    end
    
    """
    `get_result() -> Any`
    
    access _cppcall result
    """
    function get_result() ::Any

        return _cppcall._state[]._result
    end
    
    """
    `set_arguments(xs...) -> Nothing`
    
    modify _cppcall state argument tuple
    """
    function set_arguments(xs...) ::Nothing

        global _cppcall._state[]._arguments = xs
        return nothing
    end
    
    """
    `get_result() -> Tuple`
    
    access _cppcall state argument tuple
    """
    function get_arguments() ::Tuple

        return _cppcall._state[]._arguments
    end

    """
    `verify_library() -> Bool`

    check if c_adapter library is available
    """
    function verify_library() ::Bool

        for name in readdir()
            if name == "libjluna_c_adapter.so"
                return true
            end
        end

        message = "when trying to initialize jluna.cppcall: "
        message *= "cannot find \"libjluna_c_adapter.so\" in executable directory " * string(pwd())

        println(sprint(Base.showerror, AssertionError(message), backtrace()))
        return false
    end
end

"""
`cppcall(::Symbol, ::Any...) -> Any`

Call a lambda registered via `jluna::State::register_function`
and forward the `xs...` to it.
After the C++-side function returns, return the resulting object
(or `nothing` if the C++ function returns `void`)
"""
function cppcall(function_name::Symbol, xs...) ::Any

    id = hash(function_name)

    if (!ccall((:is_registered, _cppcall._library_name), Bool, (Csize_t,), id))
        throw(_cppcall.UnregisteredFunctionNameException(function_name))
        #ccall((:throw_undefined_symbol, _cppcall._library_name), Cvoid, (Cstring,), string(function_name))
    end

    _cppcall.set_arguments(xs...)
    _cppcall.set_result(nothing)

    ccall((:call_function, _cppcall._library_name), Cvoid, (Csize_t,), id)

    return _cppcall.get_result()
end
export cppcall
)";
}

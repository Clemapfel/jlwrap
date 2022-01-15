#
# Copyright 2021 Clemens Cords
# Created on 17.12.21 by clem (mail@clemens-cords.com)
#
begin

    """
    get value type of array

    @param _: array of any rank
    @returns value type T
    """
    function get_value_type_of_array(_::Array{T}) where {T}

        return T
    end

    """
    check wether a string can be transformed into a base 10 number

    @param x: string
    @returns bool
    """
    function is_number_only(x::String) ::Bool

        for s in x
            if s != '0' || s != '1' || s != '2' || s != '3' || s != '4' || s != '5' || s != '6' || s != '7' || s != '8' || s != '9'
                return false
            end
        end

        return true;
    end

    """
    check if method of given function is available for a specific variable

    @param f: function
    @param variable
    @returns true if method is available, false otherwise
    """
    function is_method_available(f::Function, variable) ::Bool

        return hasmethod(f, Tuple{typeof(variable)})
    end

    """
    check if element exists in array

    @param array
    @param v: element
    @returns bool
    """
    function exists(array::T, v::Any) ::Bool where T <: AbstractArray

        return !isempty(findall(x -> x == v, array))
    end

    """
    get nth element of tuple

    @param x: tuple
    @param i: index
    @returns element
    """
    function tuple_at(x::Tuple, i::Integer)
        return x[i]
    end

    """
    wrap vector ctor in varargs argument, used by box/unbox
    """
    function make_vector(args::T...) ::Vector{T} where T

        return [args...]
    end

    """
    wrap function call for non-function objects
    """
    function invoke(x::Any, args...) ::Any
        return x(args...)
    end

    """
    transform collection into array
    """
    function serialize(x::IdDict{Key_t, Value_t}) ::Vector{Pair{Key_t, Value_t}} where {Key_t, Value_t}

        out = Vector{Pair{Key_t, Value_t}}()
        for e in x
            push!(out, e)
        end
        return out;
    end

    function serialize(x::Set{T}) ::Vector{T} where T

        out = Vector{T}()

        for e in x
            push!(out, e)
        end
        return out;
    end

    """
    wrap dot operator for both modules, structs and arrays

    @param x: Array
    @param field_name
    @returns field
    """
    function dot(x::Array, field_name::Symbol) ::Any

        index_maybe = parse(Int, string(symbol));
        @assert index_maybe isa Integer
        return x[index_maybe];
    end
    export dot;

    dot(x::Module, field_name::Symbol) = return x.eval(field_name);
    dot(x::Any, field_name::Symbol) = return eval(:($x.$field_name))

    """
    """
    function assemble_name(names::Symbol...) ::String

        assembled = ""

        for (i, s) in enumerate(names)

            as_string = string(s)
            if i != 1 && as_string[1] != '['
                assembled *= "."
            end

            assembled *= as_string
        end

        return assembled;
    end

    """
    used by jluna::Proxy to reassemble the full variable name and then assign it
    """
    function assemble_assign(new_value::Any, names::Symbol...) ::Nothing

        Main.eval(Expr(:(=), Meta.parse(assemble_name(names...)), new_value));
        return nothing
    end

    """
    used by jluna::Proxy to access member
    """
    function assemble_dot(new_value::Any, names::Symbol...) ::Any

        return Main.eval(Meta.parse(assemble_name(names...)));
    end

    """
    transform a quote block to an identical :() expression by removing the first quote node
    """
    macro unquote(expr::Expr)

        function aux!(args::Vector{Any}) ::Nothing

            to_delete = Vector{Integer}()
            for (i, x) in enumerate(args)
                if x isa LineNumberNode
                    push!(to_delete, i)
                elseif x isa Expr
                    aux!(x.args)
                end
            end

            n_deleted = 0;
            for i in to_delete
                deleteat!(args, i - n_deleted)
                n_deleted += 1
            end
        end

        aux!(expr.args)
        return Expr(expr.head, :($(expr.args...)))
    end
end


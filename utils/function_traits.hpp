#pragma once

#include <type_traits>
#include <functional>
#include "tmp.hpp"

namespace cu {
namespace tmp {

template<typename F, std::size_t ArgOffset = 0>
struct function_traits;

template<typename R, typename... Args, std::size_t ArgOffset>
struct function_traits<R(*)(Args...), ArgOffset> : function_traits<R(Args...)> {};

template<typename R, typename... Args, std::size_t ArgOffset>
struct function_traits<R(Args...), ArgOffset> {
    typedef R result_type;
    static constexpr std::size_t arity = sizeof...(Args) - ArgOffset;

    template<std::size_t Index>
    struct argument {
        static_assert(Index < arity, "Index is larger than amount of arguments." );
        typedef typename std::tuple_element<Index + ArgOffset, std::tuple<Args...>>::type type;
    };
};

template<typename Class, typename R>
struct function_traits<R(Class::*)> : function_traits<R(Class&)> {};

template<typename Class, typename R, typename... Args>
struct function_traits<R(Class::*)(Args...)> : function_traits<R(Class&,Args...), 1> {};

template<typename Class, typename R, typename... Args>
struct function_traits<R(Class::*)(Args...) const> : function_traits<R(Class&,Args...), 1> {};

template<typename Functor>
struct functor_traits : function_traits<decltype(&Functor::operator())> {};

} // namespace tmp
} // namespace cu

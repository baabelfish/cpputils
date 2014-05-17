#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

// FIXME: Remove these macros. Some day...

#define _ChainableUtilitiesWOP(NAMESPACE, FUNC)\
inline Chain<C>& FUNC() {\
    m_v = std::move(NAMESPACE::FUNC(std::move(m_v)));\
    return *this;\
}

#define _ChainableUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline Chain<C>& FUNC(Args... args) {\
    m_v = std::move(NAMESPACE::FUNC(std::move(m_v), std::move(std::forward<Args>(args)...)));\
    return *this;\
}

#define _ReturningUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline auto FUNC(Args... args) -> decltype(NAMESPACE::FUNC(m_v, std::forward<Args>(args)...)) {\
    return NAMESPACE::FUNC(m_v, std::forward<Args>(args)...);\
}


namespace cu {

template<class C, typename T = typename C::value_type>
class Chain {
    C m_v;

public:
    Chain(C v):
        m_v(std::move(v)) {}
    virtual ~Chain() {}
    virtual Chain<C> copy() const { return *this; }

    template<typename F>
    Chain<C>& custom(F f) {
        m_v = std::move(f(std::move(m_v)));
        return *this;
    }

    virtual C&& values() {
        return std::move(m_v);
    }

    _ChainableUtilities(cf, listen)
    _ChainableUtilities(cf, tap)
    _ChainableUtilities(cu, at)
    _ChainableUtilities(cu, concat)
    _ChainableUtilities(cu, prepend)

    _ChainableUtilities(cf, pipe)
    _ChainableUtilities(cu, filter)
    _ChainableUtilities(cu, map)
    _ChainableUtilities(cu, reject)

    _ChainableUtilitiesWOP(cu, reverse)
    _ChainableUtilitiesWOP(cu, sort)
    _ChainableUtilitiesWOP(cu, unique)

    _ReturningUtilities(cu, fold)
    _ReturningUtilities(cu, frequencies)
    _ReturningUtilities(cu, groupBy)
    _ReturningUtilities(cu, rfold)

    _ReturningUtilities(cu, all)
    _ReturningUtilities(cu, any)
    _ReturningUtilities(cu, areEqual)
    _ReturningUtilities(cu, contains)
    _ReturningUtilities(cu, none)

    _ReturningUtilities(cu, find)
    _ReturningUtilities(cu, findLast)
    _ReturningUtilities(cu, first)
    _ReturningUtilities(cu, last)
    _ReturningUtilities(cu, max)
    _ReturningUtilities(cu, min)
    _ReturningUtilities(cu, minmax)
    _ReturningUtilities(cu, size)

};

} // namespace cu

template<typename C>
cu::Chain<C> _(C v) { return std::move(cu::Chain<C>(std::move(v))); }

template<typename T>
cu::Chain<std::vector<T>> _(std::initializer_list<T> v) { return std::move(_(std::vector<T>(v))); }

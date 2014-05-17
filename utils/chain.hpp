#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

// FIXME: Remove these macros. Some day...

#define _StaticUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline static auto FUNC(Args... args) -> decltype(NAMESPACE::FUNC(std::forward<Args>(args)...)) {\
    return NAMESPACE::FUNC(std::forward<Args>(args)...);\
}

#define _ChainableUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline Chain<C>& FUNC(Args... args) {\
    m_fs.push_back([=](C v) { return std::move(NAMESPACE::FUNC(std::move(v), args...)); });\
    return *this;\
}

#define _ReturningUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline auto FUNC(Args... args) -> decltype(NAMESPACE::FUNC(m_v, std::forward<Args>(args)...)) {\
    run();\
    return NAMESPACE::FUNC(m_v, std::forward<Args>(args)...);\
}


namespace cu {

template<class C, typename T = typename C::value_type>
class Chain {
    typedef std::function<C(C v)> FunctionType;
    std::vector<FunctionType> m_fs;
    C m_v;

    void run() {
        for (auto& x : m_fs) { m_v = std::move(x(std::move(m_v))); }
    }

public:
    Chain(C v):
        m_fs(),
        m_v(std::move(v)) {}
    virtual ~Chain() {}
    virtual Chain<C> copy() const { return *this; }

    Chain<C>& custom(FunctionType f) {
        m_fs.push_back(f);
        return *this;
    }

    virtual C&& values() {
        run();
        return std::move(m_v);
    }

    _StaticUtilities(cf, after)
    _StaticUtilities(cf, defer)
    _StaticUtilities(cf, delay)
    _StaticUtilities(cf, once)
    _StaticUtilities(cf, uniqueId)
    _StaticUtilities(cf, wait)
    _StaticUtilities(cf, wrap)

    _ChainableUtilities(cf, listen)
    _ChainableUtilities(cf, pipe)
    _ChainableUtilities(cf, tap)
    _ChainableUtilities(cu, at)
    _ChainableUtilities(cu, concat)
    _ChainableUtilities(cu, filter)
    _ChainableUtilities(cu, map)
    _ChainableUtilities(cu, prepend)
    _ChainableUtilities(cu, reject)
    _ChainableUtilities(cu, reverse)
    _ChainableUtilities(cu, sort)
    _ChainableUtilities(cu, unique)

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

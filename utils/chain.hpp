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
inline Chain<T>& FUNC(Args... args) {\
    m_fs.push_back([=](T v) { return std::move(NAMESPACE::FUNC(std::move(v), args...)); });\
    return *this;\
}

#define _ReturningUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline auto FUNC(Args... args) -> decltype(NAMESPACE::FUNC(m_v, std::forward<Args>(args)...)) {\
    run();\
    return NAMESPACE::FUNC(m_v, std::forward<Args>(args)...);\
}


namespace cu {

template<class T>
class Chain {
    typedef std::function<T(T v)> FunctionType;
    std::vector<FunctionType> m_fs;
    T m_v;

    void run() {
        for (auto& x : m_fs) { m_v = std::move(x(std::move(m_v))); }
    }

public:
    Chain(T v):
        m_fs(),
        m_v(v) {}
    virtual ~Chain() {}

    virtual Chain<T> copy() const { return *this; }

    Chain<T>& custom(FunctionType f) {
        m_fs.push_back(f);
        return *this;
    }

    virtual T&& values() {
        run();
        return std::move(m_v);
    }

    _StaticUtilities(cf, delay)
    _StaticUtilities(cf, after)
    _StaticUtilities(cf, once)
    _StaticUtilities(cf, wrap)
    _StaticUtilities(cf, wait)
    _StaticUtilities(cf, defer)
    _StaticUtilities(cf, uniqueId)

    _ChainableUtilities(cf, listen)
    _ChainableUtilities(cf, pipe)
    _ChainableUtilities(cf, tap)
    _ChainableUtilities(cu, at)
    _ChainableUtilities(cu, filter)
    _ChainableUtilities(cu, map)
    _ChainableUtilities(cu, reject)
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

template<typename T>
cu::Chain<T> _() {
    return cu::Chain<T>();
}

template<typename T>
cu::Chain<T> _(T v) {
    return cu::Chain<T>(v);
}

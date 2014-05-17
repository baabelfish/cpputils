#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

// FIXME: Remove these macros. Some day...

#define _ChainableUtilitiesWOP(NAMESPACE, FUNC)\
inline Chain<C>& FUNC() {\
    m_fs.push_back([=](C v) { return std::move(NAMESPACE::FUNC(std::move(v))); });\
    return *this;\
}

#define _ChainableUtilitiesWP(NAMESPACE, FUNC)\
template<typename P>\
inline Chain<C>& FUNC(P f) {\
    m_fs.push_back([=](C v) { return std::move(NAMESPACE::FUNC(std::move(v), std::move(f))); });\
    return *this;\
}

#define _ChainableUtilities(NAMESPACE, FUNC)\
template<typename... Args>\
inline Chain<C>& FUNC(Args... args) {\
    m_fs.push_back([=](C v) { return std::move(NAMESPACE::FUNC(std::move(v), std::move(args...))); });\
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

#ifdef __clang__
    _ChainableUtilities(cf, listen)
    _ChainableUtilities(cf, tap)
    _ChainableUtilities(cu, at)
    _ChainableUtilities(cu, concat)
    _ChainableUtilities(cu, prepend)
#endif

    _ChainableUtilitiesWP(cf, pipe)
    _ChainableUtilitiesWP(cu, filter)
    _ChainableUtilitiesWP(cu, map)
    _ChainableUtilitiesWP(cu, reject)
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

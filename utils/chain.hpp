#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

// FIXME: Remove these macros. Some day...

#define _ChainableUtilities(FUNC)\
template<typename F>\
Chain<T>& FUNC(F f) {\
    m_fs.push_back([=](T v) { return std::move(cu::FUNC(std::move(v), f)); });\
    return *this;\
}

#define _ReturningUtilities(FUNC)\
template<typename... Args>\
auto FUNC(Args... args) -> decltype(cu::FUNC(m_v, std::forward<Args>(args)...)) {\
    run();\
    return cu::FUNC(m_v, std::forward<Args>(args)...);\
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

    virtual T&& value() {
        run();
        return std::move(m_v);
    }

    _ChainableUtilities(map)
    _ChainableUtilities(filter)
    _ChainableUtilities(reject)
    _ChainableUtilities(sort)
    _ChainableUtilities(at)
    _ChainableUtilities(unique)
    _ChainableUtilities(tap)
    _ChainableUtilities(listen)
    _ChainableUtilities(pipe)

    _ReturningUtilities(groupBy)
    _ReturningUtilities(frequencies)
    _ReturningUtilities(fold)
    _ReturningUtilities(rfold)

    _ReturningUtilities(all)
    _ReturningUtilities(any)
    _ReturningUtilities(none)
    _ReturningUtilities(areEqual)
    _ReturningUtilities(contains)

    _ReturningUtilities(size)
    _ReturningUtilities(min)
    _ReturningUtilities(max)
    _ReturningUtilities(minmax)
    _ReturningUtilities(find)
    _ReturningUtilities(findLast)
    _ReturningUtilities(first)
    _ReturningUtilities(last)
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

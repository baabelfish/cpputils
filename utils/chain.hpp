#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

#define _ChainableUtilities(FUNC)\
template<typename F>\
Chain<T>& FUNC(F f) {\
    m_fs.push_back([=](T v) { return std::move(cu::FUNC(std::move(v), f)); });\
    return *this;\
}


#define _ReturningUtilities(FUNC)\
template<typename F>\
auto FUNC(F f) -> decltype(cu::FUNC(m_v, f)) {\
    run();\
    return cu::FUNC(m_v, f);\
}


#define _ReturningWOPUtilities(FUNC)\
auto FUNC() -> decltype(cu::FUNC(m_v)) {\
    run();\
    return cu::FUNC(m_v);\
}


// TODO:
// custom function

namespace cu {

template<class T>
class Chain {
    typedef std::function<T(T v)> function_type;
    std::vector<function_type> m_fs;
    T m_v;

    void run() {
        for (auto& x : m_fs) { m_v = std::move(x(std::move(m_v))); }
    }

public:
    typedef T value_type;

    Chain(T v):
        m_fs(),
        m_v(v) {}
    virtual ~Chain() {}

    virtual T&& value() {
        run();
        return std::move(m_v);
    }

    virtual Chain<T> copy() const { return *this; }

    _ChainableUtilities(map)
    _ChainableUtilities(filter)

    _ReturningUtilities(all)
    _ReturningUtilities(any)
    _ReturningUtilities(none)

    _ReturningWOPUtilities(size)
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

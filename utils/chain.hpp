#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

// TODO:
// custom function

namespace cu {

template<class T>
class Chain {
    typedef std::function<T(T v)> function_type;

    T m_v;
    std::vector<function_type> m_fs;

public:
    typedef T value_type;

    Chain(T v):
        m_v(v),
        m_fs() {}
    virtual ~Chain() {}
    T&& value() {
        for (auto& x : m_fs) {
            m_v = std::move(x(std::move(m_v)));
        }
        return std::move(m_v);
    }
    Chain<T> copy() const { return *this; }

    template<typename F>
    Chain<T>& map(F f) {
        m_fs.push_back([=](T m_v) {
            return std::move(cu::map(std::move(m_v), f));
        });
        return *this;
    }

};

} // namespace cu

template<typename T>
cu::Chain<T> _(T v) {
    return cu::Chain<T>(v);
}

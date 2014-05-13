#pragma once

#include <utility>
#include "associative_containers.hpp"
#include "chain.hpp"
#include "containers.hpp"
#include "functions.hpp"

template<class T>
class Chain {
    T v;

public:
    typedef T value_type;

    Chain(T v): v(v) {}
    virtual ~Chain() {}
    T&& value() { return std::move(v); }
};

template<typename T>
Chain<T> _(T v) {
    return Chain<T>(v);
}

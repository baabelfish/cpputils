#pragma once

#include <functional>
#include <iostream>

namespace cf {
namespace internal {

void _print() { std::cout << std::endl; }

template<typename T>
void _print(T t) { std::cout << t << std::endl; }

template<typename T, typename... Args>
void _print(T t, Args... args) {
    std::cout << t << " ";
    _print(std::forward<Args>(args)...);
}

} // namespace internal

template<typename... Args>
void print(Args... args) {
    internal::_print(std::forward<Args>(args)...);
}

} // namespace cf

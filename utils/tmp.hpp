#pragma once

#include <tuple>
#include <functional>

namespace cu {
namespace tmp {

template<typename T, typename... Args>
inline T head(T t, Args...) {
    return std::forward<T>(t);
}

template<typename... Args>
std::size_t amount(Args... args) {
    return sizeof...(args);
}

template<typename T>
inline T nth(std::size_t, T h) { return h; }

template<typename T, typename... Args>
inline T nth(std::size_t n, T h, Args... args) {
    return n == 0 ? h : nth(n - 1, std::forward<Args>(args)...);
}

} // namespace tmp
} // namespace cu

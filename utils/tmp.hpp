#pragma once

#include <functional>

namespace cu {
namespace tmp {

template<typename T, typename... Args>
T head(T t, Args...) {
    return std::forward<T>(t);
}

template<typename T, typename Y>
struct isSame { static const bool value = false; };

template<typename T>
struct isSame<T, T> { static const bool value = true; };

} // namespace tmp
} // namespace cu

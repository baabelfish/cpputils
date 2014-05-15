#pragma once

#include <functional>

namespace cu {
namespace tmp {

template<typename T, typename... Args>
T head(T t, Args...) {
    return std::forward<T>(t);
}

} // namespace tmp
} // namespace cu

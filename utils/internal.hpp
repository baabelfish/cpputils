#pragma once

#include <functional>

namespace cu {
namespace internal {

template<typename C>
inline void at(C& n, C c, std::size_t i) {
    n.insert(n.end(), c[i]);
}

template<typename C, typename... Args>
inline void at(C& n, C c, std::size_t i, Args... args) {
    at(n, c, i);
    at(n, c, std::forward<Args>(args)...);
}

} // namespace internal
} // namespace cu

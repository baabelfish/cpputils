#pragma once

#include <tuple>
#include <functional>
#include <type_traits>
#include <iterator>

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

template <typename T>
using enable_if = typename std::enable_if<T::value, void>::type;

template <typename T, typename Tag>
using has_iterator_tag = std::is_same<typename std::iterator_traits<typename T::iterator>::iterator_category, Tag>;

template <typename T>
using has_random_access_iterators = has_iterator_tag<T, std::random_access_iterator_tag>;

template <typename T>
using has_bidirectional_iterators = has_iterator_tag<T, std::bidirectional_iterator_tag>;

template <typename T>
using has_forward_iterators = has_iterator_tag<T, std::forward_iterator_tag>;

template <typename T>
using has_output_iterators = has_iterator_tag<T, std::output_iterator_tag>;

template <typename T>
using has_input_iterators = has_iterator_tag<T, std::input_iterator_tag>;

} // namespace tmp
} // namespace cu

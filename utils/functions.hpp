#pragma once

// TODO:
// debounce
// memoize
// throttle
// partial
// compose (just use lambdas?)

#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include <iostream>
#include <string>
#include <future>
#include "internal.hpp"

namespace cf {

template<typename F, typename... Args>
inline cu::internal::After<F, Args...> after(std::size_t amount, F f, Args... args) {
    return std::move(cu::internal::After<F, Args...>(amount, f, std::forward<Args>(args)...));
}

template<typename F>
inline cu::internal::Once<F> once(F f) {
    return std::move(cu::internal::Once<F>(f));
}

template<typename T, typename F>
inline cu::internal::Wrap<T, F> wrap(T t, F f) {
    return std::move(cu::internal::Wrap<T, F>(t, f));
}

inline void wait(std::size_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

template<typename F, typename... Args>
inline auto defer(F f, Args... args) -> decltype(std::async(std::launch::deferred, f)) {
    return std::async(std::launch::deferred, [](F f, Args... args) {
        f(std::forward<Args>(args)...);
    },
    f, std::forward<Args>(args)...);
}

template<typename F, typename... Args>
inline auto delay(std::size_t ms, F f, Args... args) -> decltype(std::async(std::launch::async, f)) {
    return std::async(std::launch::async, [](std::size_t ms, F f, Args... args) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        f(std::forward<Args>(args)...);
    }, ms, f, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline T identity(T v, Args...) {
    return std::move(v);
}

inline std::string uniqueId(std::string prefix) { return prefix + std::to_string(cu::internal::uniqueId()); }
inline std::wstring uniqueId(std::wstring prefix) { return prefix + std::to_wstring(cu::internal::uniqueId()); }
inline std::size_t uniqueId() { return cu::internal::uniqueId(); }

template<typename F, typename... Args>
inline void times(std::size_t amount, F f, Args... args) {
    for (std::size_t i = 0; i < amount; ++i) { f(std::forward<Args>(args)...); }
}

template<typename T, typename F>
inline T tap(T v, F f) {
    f(v);
    return std::move(v);
}

template<typename T>
inline T listen(T v) {
    return tap(v, [](const T& t) { std::cout << t << std::endl; });
}

template<typename T, typename F>
inline T pipe(T v, F f) {
    return f(v);
}

template<typename T, typename F, typename... Args>
inline T pipe(T v, F f, Args... args) {
    return f(v, std::forward<Args>(args)...);
}

} // namespace cu

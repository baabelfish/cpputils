#pragma once

#include <functional>
#include <iostream>

namespace cu {

class after {
    std::function<void()> m_f;
    std::size_t m_calls = 0;
    std::size_t m_amount;

public:
    after(std::size_t amount, std::function<void()> f):
        m_f(f),
        m_calls(0),
        m_amount(amount) {
        if (amount == 0) { f(); }
    }
    void operator()() {
        if (++m_calls == m_amount) { m_f(); }
    }
};

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

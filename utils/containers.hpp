#pragma once

#include <algorithm>

namespace cu {

template<typename C, typename T = typename C::value_type>
inline T first(C vec) {
    return *vec.begin();
}

template<typename C, typename T = typename C::value_type>
inline T last(C vec) {
    return *vec.rbegin();
}

template<typename R, typename C, typename F>
inline R map(C c, F f) {
    R r;
    r.resize(c.size());
    std::transform(begin(c), end(c), begin(r), f);
    return r;
}

template<typename C, typename F>
inline C map(C c, F f) {
    for (auto& x : c) { x = f(x); }
    return c;
}

template<typename C, typename T = typename C::value_type, typename F>
inline void each(C& c, F f) {
    std::for_each(c.begin(), c.end(), f);
}

template<typename C, typename F>
inline C filter(C c, F f) {
    C n;
    for (auto& x : c) {
        if (f(x)) { n.push_back(x); }
    }
    return n;
}

template<typename C, typename T = typename C::value_type, typename F>
inline T fold(C c, F f) {
    T temp = T();
    if (c.size() == 1) {
        return *c.begin();
    } else if (c.size() > 1) {
        auto cur = ++c.begin();
        temp = f(*c.begin(), *cur);
        ++cur;
        while (cur != c.end()) {
            temp = f(temp, *cur);
            ++cur;
        }
    }
    return temp;
}

template<typename C, typename T = typename C::value_type, typename F>
inline T rfold(C c, F f) {
    T temp = T();
    if (c.size() == 1) {
        return *c.begin();
    } else if (c.size() > 1) {
        auto cur = ++c.rbegin();
        temp = f(*c.rbegin(), *cur);
        ++cur;
        while (cur != c.rend()) {
            temp = f(temp, *cur);
            ++cur;
        }
    }
    return temp;
}

template<typename C, typename F>
inline bool all(C c, F f) {
    return std::all_of(begin(c), end(c), f);
}

template<typename C, typename F>
inline bool any(C c, F f) {
    return std::any_of(begin(c), end(c), f);
}

template<typename C, typename F>
inline bool none(C c, F f) {
    return std::none_of(begin(c), end(c), f);
}

} // namespace cu

#pragma once

#include <set>
#include <forward_list>
#include <unordered_set>
#include <tuple>
#include <list>
#include <deque>
#include <algorithm>
#include "internal.hpp"
#include "datastructures/minmax.hpp"

// TODO:
// without
// union
// difference
// intersection
// zip
// lazyrange (with c++17)

// sample
// invoke

#define __ContainerConstructorHelper(NAME,TYPE)\
template<typename T, typename... Args>\
inline std::TYPE<T> NAME(T t, Args... args) { return std::TYPE<T>{t, args...}; }

#define __ContainerConvertHelperSet(NAME,TYPE)\
template<typename C, typename T = typename C::value_type>\
inline std::TYPE<T> NAME(C c) {\
    std::TYPE<T> n;\
    n.insert(std::make_move_iterator(c.begin()), std::make_move_iterator(c.end()));\
    return n;\
}

#define __ContainerConvertHelper(NAME,TYPE)\
template<typename C, typename T = typename C::value_type>\
inline std::TYPE<T> NAME(C c) {\
    std::TYPE<T> n;\
    n.insert(n.begin(), std::make_move_iterator(c.begin()), std::make_move_iterator(c.end()));\
    return n;\
}

namespace cu {

__ContainerConstructorHelper(set,set)
__ContainerConstructorHelper(multiset,multiset)
__ContainerConstructorHelper(uset,unordered_set)
__ContainerConstructorHelper(multiuset,unordered_multiset)
__ContainerConstructorHelper(flist,forward_list)
__ContainerConstructorHelper(list,list)
__ContainerConstructorHelper(deque,deque)
__ContainerConstructorHelper(vec,vector)

__ContainerConvertHelperSet(set,set)
__ContainerConvertHelperSet(multiset,multiset)
__ContainerConvertHelperSet(uset,unordered_set)
__ContainerConvertHelperSet(multiuset,unordered_multiset)

__ContainerConvertHelper(list,list)
__ContainerConvertHelper(deque,deque)
__ContainerConvertHelper(vec,vector)

template<typename C, typename T = typename C::value_type>
inline std::forward_list<T> flist(C c) {
    std::forward_list<T> n;
    n.insert_after(n.before_begin(), std::make_move_iterator(c.begin()), std::make_move_iterator(c.end()));
    return n;
}


template<typename C>
inline C reverse(C c) {
    std::reverse(c.begin(), c.end());
    return std::move(c);
}

template<typename C>
inline C prepend(C c) { return std::move(c); }
template<typename C, typename T = typename C::value_type, typename CF, typename... Containers>
inline C prepend(C c, CF other, Containers... rest) {
    c.insert(c.begin(), std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
    return prepend(std::move(c), std::forward<Containers>(rest)...);
}
template<typename T, typename CF, typename... Containers>
inline std::forward_list<T> prepend(std::forward_list<T> c, CF other, Containers... rest) {
    c.insert_after(c.before_begin(), std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
    return prepend(std::move(c), std::forward<Containers>(rest)...);
}

template<typename C>
inline C concat(C c) { return std::move(c); }

template<typename C, typename T = typename C::value_type, typename CF, typename... Containers>
inline C concat(C c, CF other, Containers... rest) {
    c.resize(c.size() + other.size());
    std::move_backward(other.begin(), other.end(), c.end());
    return concat(std::move(c), std::forward<Containers>(rest)...);
}

template<typename C, typename T = typename C::value_type>
inline T min(C c) {
    return *std::min_element(c.begin(), c.end());
}

template<typename C, typename T = typename C::value_type>
inline T max(C c) {
    return *std::max_element(c.begin(), c.end());
}

template<typename C, typename T = typename C::value_type>
inline ds::MinMax<T> minmax(C c) {
    auto res = std::minmax_element(c.begin(), c.end());
    return { *res.first, *res.second };
}

template<typename T>
inline std::vector<T> range(T start, T stop, T step = 1) {
    std::vector<T> r;
    while (start < stop) {
        r.push_back(start);
        start += step;
    };
    return std::move(r);
}

template<typename T, typename Validator, typename Generator>
inline std::vector<T> range(T start, Validator v, Generator g) {
    std::vector<T> r;
    while (v(start)) {
        r.push_back(start);
        start = g(start);
    };
    return std::move(r);
}

template<typename R, typename C, typename F, typename T = typename C::value_type>
inline std::unordered_map<R, std::vector<T>> groupBy(C c, F f) {
    std::unordered_map<R, std::vector<T>> groups;
    for (auto& x : c) {
        groups[f(x)].push_back(x);
    }
    return std::move(groups);
}

template<typename C, typename T = typename C::value_type>
inline std::unordered_map<T, std::size_t> frequencies(C c) {
    std::unordered_map<T, std::size_t> freqs;
    for (auto& x : c) {
        auto it = freqs.find(x);
        if (it == freqs.end()) { freqs[x] = 1; }
        else { it->second += 1; }
    }
    return std::move(freqs);
}

template<typename T>
inline std::unordered_map<T, std::size_t> frequencies(std::initializer_list<T> c) {
    return frequencies<std::initializer_list<T>>(c);
}

template<typename C, typename F>
inline C sort(C c, F f) {
    std::sort(c.begin(), c.end(), f);
    return std::move(c);
}

template<typename C>
inline C sort(C c) {
    std::sort(c.begin(), c.end());
    return std::move(c);
}

template<typename C1, typename C2, typename T1 = typename C1::value_type, typename T2 = typename C2::value_type, typename F>
inline bool areEqual(C1 r1, C2 r2, F f) {
    auto it1 = r1.begin();
    auto it2 = r2.begin();
    while (it1 != r1.end() && it2 != r2.end()) {
        if (!f(*it1, *it2)) { return false; }
        ++it1;
        ++it2;
    }
    return it1 == r1.end() && it2 == r2.end();
}

template<typename C1, typename C2, typename T1 = typename C1::value_type, typename T2 = typename C2::value_type>
inline bool areEqual(C1 r1, C2 r2) {
    return areEqual(r1, r2, [](const T1& a, const T2& b) { return a == b; });
}

template<typename T, typename Y, typename F>
inline bool areEqual(std::initializer_list<T> r1, std::initializer_list<Y> r2, F f) {
    return areEqual<std::initializer_list<T>, std::initializer_list<Y>>(r1, r2, f);
}

template<typename T, typename Y>
inline bool areEqual(std::initializer_list<T> r1, std::initializer_list<Y> r2) {
    return areEqual<std::initializer_list<T>, std::initializer_list<Y>>(r1, r2);
}

template<typename C, typename T = typename C::value_type>
inline C unique(C c) {
    if (std::is_sorted(c.begin(), c.end())) {
        auto lit = std::unique(c.begin(), c.end());
        c.resize(std::distance(c.begin(), lit));
    } else {
        C n;
        std::unordered_set<T> uniq;
        auto it = c.begin();
        auto rit = c.begin();
        while (it != c.end()) {
            if (uniq.find(*it) == uniq.end()) {
                *rit = *it;
                ++rit;
                uniq.insert(*it);
            }
            ++it;
        }
        c.resize(std::distance(c.begin(), rit));
    }
    return std::move(c);
}

template<typename T, typename... Args>
inline bool contains(T t, Args... args) {
    return internal::contains(t, std::forward<Args>(args)...);
}

template<typename C, typename T = typename C::value_type, typename F>
inline typename C::const_reverse_iterator findLast(const C& c, F f) {
    for (auto it = c.rbegin(); it != c.rend(); ++it) {
        if (f(*it)) { return it; }
    }
    return c.rend();
}

template<typename C, typename T = typename C::value_type, typename F>
inline typename C::const_iterator find(const C& c, F f) {
    for (auto it = c.begin(); it != c.end(); ++it) {
        if (f(*it)) { return it; }
    }
    return c.end();
}

template<typename C, typename... Args>
inline C at(C c, Args... args) {
    C n;
    internal::at(n, c, std::forward<Args>(args)...);
    return std::move(n);
}

// TODO: Add optional n
template<typename C, typename T = typename C::value_type>
inline T first(C vec) {
    return *vec.begin();
}

// TODO: Add optional n
template<typename C, typename T = typename C::value_type>
inline T last(C vec) {
    return *vec.rbegin();
}

template<typename C>
inline std::size_t size(const C& c) {
    return c.size();
}

template<typename R, typename C, typename F>
inline R map(C c, F f) {
    R r;
    r.resize(c.size());
    std::transform(begin(c), end(c), begin(r), f);
    return std::move(r);
}

template<typename C, typename F>
inline C map(C c, F f) {
    for (auto& x : c) { x = f(x); }
    return std::move(c);
}

template<typename C, typename T = typename C::value_type, typename F>
inline void each(C& c, F f) {
    std::for_each(c.begin(), c.end(), f);
}

template<typename C, typename F>
inline C filter(C c, F f) {
    C n;
    for (auto& x : c) {
        if (f(x)) { n.insert(n.end(), x); }
    }
    return std::move(n);
}

template<typename C, typename F>
inline C reject(C c, F f) {
    C n;
    for (auto& x : c) {
        if (!f(x)) { n.insert(n.end(), x); }
    }
    return std::move(n);
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

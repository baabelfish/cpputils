#pragma once

#include <vector>
#include <mutex>
#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "tmp.hpp"

namespace cu {
namespace internal {

template<typename R, typename KC, typename VC, typename K = typename KC::value_type, typename V = typename VC::value_type>
inline R abstractZip(KC kc, VC vc) {
    R m;
    auto kit = kc.begin();
    auto vit = vc.begin();
    while (kit != kc.end() && vit != vc.end()) {
        m.insert(std::pair<K, V>(*kit, *vit));
        ++kit;
        ++vit;
    }
    return m;
}

namespace aux {

class Uniquable {
protected:
    static std::size_t uniqueId() {
        static std::mutex m;
        std::lock_guard<std::mutex> l(m);
        static std::size_t ui = 0;
        return ui++;
    }
};

} // namespace aux

#ifdef __clang__
template<typename F, typename... Args>
class After {
    std::function<void()> m_f;
    std::size_t m_calls;
    std::size_t m_amount;

public:
    After(std::size_t amount, F f, Args... args):
        m_f([=]() { f(args...); }),
        m_calls(0),
        m_amount(amount) {
        if (amount == 0) { m_f(); }
    }
    virtual ~After() {}
    void operator()() {
        if (++m_calls == m_amount) { m_f(); }
    }
};

template<class F>
class Once : aux::Uniquable {
    bool executed;
    std::size_t at;
    F f;

public:
    Once(F f):
        executed(false),
        at(uniqueId()),
        f(f) {}

    virtual ~Once() {}

    template<typename... Args>
    inline auto operator()(Args... args) -> decltype(f(args...)) {
        static auto re = f(args...);
        return re;
    }
};
#endif

template<typename T, typename F>
class Wrap {
    T m_t;
    F m_f;
public:
    Wrap(T t, F f):
        m_t(t),
        m_f(f) {}
    virtual ~Wrap() {}

    template<typename... Args>
    inline auto operator()(Args... args) -> decltype(m_f(m_t, std::forward<Args>(args)...)) {
        return m_f(m_t, std::forward<Args>(args)...);
    }
};

template<typename C>
inline void at(C& n, C c, std::size_t i) {
    n.insert(n.end(), c[i]);
}

template<typename C, typename... Args>
inline void at(C& n, C c, std::size_t i, Args... args) {
    at(n, c, i);
    at(n, c, std::forward<Args>(args)...);
}

template<typename T> bool contains(const std::string& s, T t) { return s.find(t) != std::string::npos; }
template<typename T> bool contains(const std::wstring& s, T t) { return s.find(t) != std::wstring::npos; }
template<typename T> bool contains(const std::u16string& s, T t) { return s.find(t) != std::u16string::npos; }
template<typename T> bool contains(const std::u32string& s, T t) { return s.find(t) != std::u32string::npos; }
template<typename T> bool contains(const char* s, T t) { return std::string(s).find(t) != std::string::npos; }
template<typename T> bool contains(const wchar_t* s, T t) { return std::wstring(s).find(t) != std::wstring::npos; }
template<typename T> bool contains(const char16_t* s, T t) { return std::u16string(s).find(t) != std::u16string::npos; }
template<typename T> bool contains(const char32_t* s, T t) { return std::u32string(s).find(t) != std::u32string::npos; }

template<typename T> bool contains(const std::set<T>& s, T t) { return s.find(t) != s.end(); }
template<typename T, typename Y> bool contains(const std::map<T, Y>& s, T t) { return s.find(t) != s.end(); }
template<typename T> bool contains(const std::unordered_set<T>& s, T t) { return s.find(t) != s.end(); }
template<typename T, typename Y> bool contains(const std::unordered_map<T, Y>& s, T t) { return s.find(t) != s.end(); }

template<typename C, typename T = typename C::value_type>
inline bool contains(const C& c, T t) {
    for (auto& x : c) {
        if (x == t) { return true; }
    }
    return false;
}

template<typename T, typename Y, typename... Args>
inline bool contains(T s, Y t, Args... args) {
    return contains(s, t) || contains(s, std::forward<Args>(args)...);
}

inline std::size_t uniqueId() {
    static std::mutex m;
    static std::size_t id = 0;
    std::lock_guard<std::mutex> l(m);
    return id++;
}

} // namespace internal
} // namespace cu

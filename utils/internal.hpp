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

template<class F>
class Once {
    bool executed;
    std::size_t at;
    F f;

public:
    Once(F f):
        executed(false),
        at(0),
        f(f) {}


    virtual ~Once() {}

    template<typename... Args>
    auto operator()(Args... args) -> decltype(f(std::forward<Args>(args)...)) {
        static std::mutex m;
        static std::vector<decltype(f(std::forward<Args>(args)...))> Revals;

        if (!executed) {
            std::lock_guard<std::mutex> l(m);
            executed = true;
            at = Revals.size();
            Revals.push_back(f(std::forward<Args>(args)...));
        }
        return Revals[at];
    }
};

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
    auto operator()(Args... args) -> decltype(m_f(m_t, std::forward<Args>(args)...)) {
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
bool contains(const C& c, T t) {
    for (auto& x : c) {
        if (x == t) { return true; }
    }
    return false;
}

template<typename T, typename Y, typename... Args>
bool contains(T s, Y t, Args... args) {
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

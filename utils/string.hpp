#pragma once

#include <iostream>
#include <string>
#include <algorithm>

// capitalize
// compareIgnoreCase
// hash
// find, ...
// replace?
// replaceAll
// to
// simplify (multiple whitespace -> one whitespace)
// cases (kebab, camel, snake)
// slice?

namespace cs {

#define _LEGACY_STRING_CONVERTER(FUNC)\
inline std::string FUNC(const char* str) { return FUNC(std::string(str)); }\
inline std::wstring FUNC(const wchar_t* str) { return FUNC(std::wstring(str)); }\
inline std::u16string FUNC(const char16_t* str) { return FUNC(std::u16string(str)); }\
inline std::u32string FUNC(const char32_t* str) { return FUNC(std::u32string(str)); }

template<typename Str>
int levenshteinDistance(const Str& s, const Str& t, const int cost_removal = 1,
                        const int cost_insert = 1, const int cost_substitute = 1) {
    const std::size_t m = s.size() + 1;
    const std::size_t n = t.size() + 1;
    std::vector<int> d(n * m, 0);

    auto at = [&](std::size_t y, std::size_t x) -> int& { return d[y * n + x]; };

    for (std::size_t i = 1; i < m; ++i) { at(i, 0) = i; }
    for (std::size_t i = 1; i < n; ++i) { at(0, i) = i; }

    for (std::size_t j = 1; j < n; ++j) {
        for (std::size_t i = 1; i < m; ++i) {
            if (s[i - 1] == t[j - 1]) {
                at(i, j) = at(i - 1, j - 1);
            }
            else {
                auto removal = at(i-1, j) + cost_removal,
                     insert = at(i, j-1) + cost_insert,
                     substitute = at(i-1, j-1) + cost_substitute;
                auto min = std::min(removal, std::min(insert, substitute));
                at(i, j) = min;
            }
        }
    }
    return at(m - 1, n - 1);
}

template<typename ST, typename T = typename ST::value_type>
inline ST toLowerCase(ST str) {
    for (auto& x : str) { x = std::tolower(x); }
    return str;
}

template<typename ST, typename T = typename ST::value_type>
inline ST toUpperCase(ST str) {
    for (auto& x : str) { x = std::toupper(x); }
    return str;
}

template<typename ST, typename T = typename ST::value_type>
inline bool beginsWith(ST str, ST what) {
    return str.find(what) == 0;
}

template<typename ST, typename T = typename ST::value_type, typename OT>
inline bool endsWith(ST str, OT what) {
    ST other(what);
    return str.rfind(other) == str.size() - other.size();
}

template<typename ST, typename T = typename ST::value_type, typename OT>
inline ST endsWith(ST str, T ch) {
    std::fill(str.begin(), str.end(), ch);
    return str;
}

template<typename C, typename T = typename C::value_type, typename OT>
inline T join(C c, OT separator) {
    T other(separator);
    T restr;
    std::size_t max = c.size();
    for (auto& x : c) {
        restr += x;
        if (--max > 0) { restr += other; }
    }
    return restr;
}

template<typename ST, typename T = typename ST::value_type, typename OT>
inline std::vector<ST> split(ST str, OT what) {
    ST other(what);
    std::vector<ST> re;
    auto it = str.find(other);
    decltype(it) old = 0;
    while (it != std::string::npos) {
        re.push_back(str.substr(old, it - old));
        old = it + other.size();
        it = str.find(other, it + other.size());
    }
    re.push_back(str.substr(old));
    return re;
}

template<typename ST, typename T = typename ST::value_type>
inline ST trim(ST str) {
    std::size_t start = 0;
    std::size_t end = str.size() - 1;
    while (start < str.size() && std::isspace(str[start])) { ++start; }
    while (end > start && std::isspace(str[end])) { --end; }
    return str.substr(start, end - start + 1);
}

_LEGACY_STRING_CONVERTER(toLowerCase)
_LEGACY_STRING_CONVERTER(toUpperCase)

} // namespace cs

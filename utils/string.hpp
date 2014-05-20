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

#pragma once

// TODO:
// merge
// pluck
// omit

#include "internal.hpp"
#include <map>
#include <unordered_map>
#include <vector>

namespace cu {

template<typename C, typename K = typename C::key_type>
inline C omit(C c, K key) {
    auto it = c.find(key);
    if (it != c.end()) {
        c.erase(it);
    }
    return std::move(c);
}

template<typename C, typename K = typename C::key_type, typename... Args>
inline C omit(C c, K key, Args... args) {
    return std::move(omit(omit(c, key), std::forward<Args>(args)...));
}

template<typename C, typename F>
inline C mapValues(C c, F f) {
    for (auto& x : c) { x.second = f(x.second); }
    return std::move(c);
}

template<typename KC, typename VC, typename K = typename KC::value_type, typename V = typename VC::value_type>
std::unordered_map<K, V> zipUnorderedMap(KC kc, VC vc) { return internal::abstractZip<std::unordered_map<K, V>>(kc, vc); }

template<typename KC, typename VC, typename K = typename KC::value_type, typename V = typename VC::value_type>
std::unordered_multimap<K, V> zipUnorderedMultiMap(KC kc, VC vc) { return internal::abstractZip<std::unordered_multimap<K, V>>(kc, vc); }

template<typename KC, typename VC, typename K = typename KC::value_type, typename V = typename VC::value_type>
std::map<K, V> zipMap(KC kc, VC vc) { return internal::abstractZip<std::map<K, V>>(kc, vc); }

template<typename KC, typename VC, typename K = typename KC::value_type, typename V = typename VC::value_type>
std::multimap<K, V> zipMultiMap(KC kc, VC vc) { return internal::abstractZip<std::multimap<K, V>>(kc, vc); }

template<typename T, typename K, typename V>
std::vector<T> keys(const std::pair<K, V>& p) {
    std::vector<T> values;
    for (auto it = p.first; it != p.second; ++it) { values.push_back(it->first); }
    return std::move(values);
}

template<typename T, typename K, typename V>
std::vector<T> values(const std::pair<K, V>& p) {
    std::vector<T> values;
    for (auto it = p.first; it != p.second; ++it) { values.push_back(it->second); }
    return std::move(values);
}

template<typename C, typename K = typename C::key_type, typename V = typename C::mapped_type>
std::vector<V> values(const C& m) {
    std::vector<V> values;
    for (auto& x : m) { values.push_back(x.second); }
    return std::move(values);
}

template<typename C, typename K = typename C::key_type, typename V = typename C::mapped_type>
std::vector<K> keys(const C& m) {
    std::vector<K> keys;
    for (auto& x : m) { keys.push_back(x.first); }
    return std::move(keys);
}

} // namespace cu

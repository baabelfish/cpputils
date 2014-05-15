#pragma once

// TODO:
// mapValues
// merge
// omit

#include "internal.hpp"
#include <map>
#include <unordered_map>
#include <vector>

namespace cu {

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
    return values;
}

template<typename T, typename K, typename V>
std::vector<T> values(const std::pair<K, V>& p) {
    std::vector<T> values;
    for (auto it = p.first; it != p.second; ++it) { values.push_back(it->second); }
    return values;
}

template<typename C, typename K = typename C::key_type, typename V = typename C::mapped_type>
std::vector<V> values(const C& m) {
    std::vector<V> values;
    for (auto& x : m) { values.push_back(x.second); }
    return values;
}

template<typename C, typename K = typename C::key_type, typename V = typename C::mapped_type>
std::vector<K> keys(const C& m) {
    std::vector<K> keys;
    for (auto& x : m) { keys.push_back(x.first); }
    return keys;
}

} // namespace cu

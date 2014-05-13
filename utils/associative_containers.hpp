#pragma once

// TODO:
// mapValues
// merge
// omit

#include <map>
#include <vector>

namespace cu {

template<typename K, typename V>
std::vector<V> values(const std::map<K, V>& m) {
    std::vector<V> values;
    for (auto& x : m) { values.push_back(x.second); }
    return values;
}

template<typename K, typename V>
std::vector<K> keys(const std::map<K, V>& m) {
    std::vector<K> keys;
    for (auto& x : m) { keys.push_back(x.first); }
    return keys;
}

} // namespace cu

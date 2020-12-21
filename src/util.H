#pragma once
#include <iostream>
#include <unordered_map>

namespace std {

template <typename K, typename V, typename... Others>
inline ostream&
operator<<(ostream& os, const unordered_map<K, V, Others...>& map) {
    os << "{";
    auto       i = cbegin(map);
    const auto e = cend(map);
    if (i != e)
        do {
            os << i->first << ": " << i->second;
            ++i;
        } while (i != e && os << ", ");
    os << "}";
    return os;
}

} // namespace std

namespace shrek {
namespace util {} // namespace util
} // namespace shrek

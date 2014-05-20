#pragma once

#include <tuple>

namespace cd {
namespace experimental {

template<typename T, typename Tuple> struct getComponent;

template<typename T, typename... Types>
struct getComponent<T, std::tuple<T, Types...>> {
    static const std::size_t index = 0;
};

template<typename T, typename O, typename... Types>
struct getComponent<T, std::tuple<O, Types...>> {
    static const std::size_t index = getComponent<T, std::tuple<Types...>>::index + 1;
};

template<typename T>
class Component {
public:
    virtual inline T& operator()() { return *static_cast<T*>(this); }
};

template<class... Types>
class Mixin {
    std::tuple<Types...> tp;

public:
    Mixin() {}
    virtual ~Mixin() {}

    template<typename T, typename... Args>
    inline auto comp(Args... args) -> decltype(std::get<getComponent<T, decltype(tp)>::index>(tp)(args...)) {
        return std::get<getComponent<T, decltype(tp)>::index>(tp)(args...);
    }
};

template<typename T, typename M, typename... Args>
inline auto comp(M m, Args... args) -> decltype(m.template comp<T>(args...)) {
    return m.template comp<T>(args...);
}

} // namespace experimental
} // namespace cd

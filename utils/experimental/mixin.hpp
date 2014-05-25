#pragma once

#include <tuple>

namespace cd {
namespace experimental {

#define MixinInterfaceVoid(CLASS, FROM, TO)\
template<typename... Args>\
inline void TO(Args... args) {\
    component<CLASS>().FROM(std::forward<Args>(args)...);\
}

#define MixinInterface(CLASS, FROM, TO)\
template<typename... Args>\
inline auto TO(Args... args) -> decltype(component<CLASS>().FROM(std::forward<Args>(args)...)) {\
    return component<CLASS>().FROM(std::forward<Args>(args)...);\
}

template<typename T, typename Tuple> struct getComponent;

template<typename T, typename... Types>
struct getComponent<T, std::tuple<T, Types...>> {
    static const std::size_t index = 0;
};

template<typename T, typename O, typename... Types>
struct getComponent<T, std::tuple<O, Types...>> {
    static const std::size_t index = getComponent<T, std::tuple<Types...>>::index + 1;
};

template<class... Types>
class Mixin {
    std::tuple<Types...> tp;

public:
    Mixin() {
        std::get<2>(tp);
    }
    virtual ~Mixin() {}

    template<typename T, typename... Args>
    inline auto component(Args... args) -> decltype(std::get<getComponent<T, decltype(tp)>::index>(tp)(args...)) {
        return std::get<getComponent<T, decltype(tp)>::index>(tp)(args...);
    }
};

template<typename T>
class Component {
public:
    virtual inline T& operator()() { return *static_cast<T*>(this); }
};

template<typename T, typename M, typename... Args>
inline auto component(M m, Args... args) -> decltype(m.template component<T>(args...)) {
    return m.template component<T>(args...);
}

} // namespace experimental
} // namespace cd

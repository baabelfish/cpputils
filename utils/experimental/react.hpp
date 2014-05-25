#pragma once

#include <functional>
#include <iostream>
#include <vector>

namespace cd {
namespace experimental {

template<typename T>
class React {
    std::vector<std::function<void(const T& t)>> before_change;
    std::vector<std::function<void(const T& t)>> after_change;
    T t;

    inline void beforeChange() const {
        for (auto& x : before_change) { x(t); }
    }

    inline void afterChange() const {
        for (auto& x : after_change) { x(t); }
    }

public:
    React(T&& t):
        before_change(),
        after_change(),
        t(std::move(t)) {
    }

    virtual ~React() {}

    friend std::ostream& operator<<(std::ostream& stream, React<T>& v) {
        return stream << v.t;
    }

    template<typename S>
    inline void operator=(S s) {
        beforeChange();
        t = s;
        afterChange();
    }

    template<typename S>
    inline auto operator+(S s) -> decltype(t + s) { return t + s; }
    template<typename S>
    inline auto operator-(S s) -> decltype(t - s) { return t - s; }
    template<typename S>
    inline auto operator*(S s) -> decltype(t * s) { return t * s; }
    template<typename S>
    inline auto operator/(S s) -> decltype(t / s) { return t / s; }

    inline const T& operator*() const {
        return t;
    }

    template<typename F>
    inline void visit(F f) {
        beforeChange();
        f(t);
        afterChange();
    }

    template<typename S>
    inline void operator/=(S s) {
        beforeChange();
        t /= s;
        afterChange();
    }

    template<typename S>
    inline void operator*=(S s) {
        beforeChange();
        t *= s;
        afterChange();
    }

    template<typename S>
    inline void operator+=(S s) {
        beforeChange();
        t += s;
        afterChange();
    }

    template<typename S>
    inline void operator-=(S s) {
        beforeChange();
        t -= s;
        afterChange();
    }

    inline React<T>& operator--() {
        beforeChange();
        --t;
        afterChange();
        return *this;
    }

    inline React<T> operator--(int) {
        beforeChange();
        afterChange();
        return React<T>(this->t--);
    }

    inline React<T>& operator++() {
        beforeChange();
        ++this->t;
        afterChange();
        return *this;
    }

    inline React<T> operator++(int) {
        beforeChange();
        afterChange();
        return React<T>(this->t++);
    }

    template<typename S>
    inline T operator%(S s) const {
        return this->t % s;
    }

    template<typename F>
    void before(F f) {
        before_change.push_back(f);
    }

    template<typename F>
    void after(F f) {
        after_change.push_back(f);
    }
};

} // namespace experimental
} // namespace cd

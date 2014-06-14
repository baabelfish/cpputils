#pragma once

#include "../function_traits.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <future>
#include <thread>

namespace ce {

class BasePromise {
public:
    BasePromise() {}
    virtual ~BasePromise() {}

protected:
    template<typename G>
    static void visitResults(G g) {
        static std::mutex mtx;
        std::lock_guard<std::mutex> l(mtx);
        static std::vector<std::future<void>> res;
        try { g(res); }
        catch (...) { res.clear(); }
    }
};

template<typename F>
class Promise : public BasePromise {
public:
    template<typename Args>
    Promise(F f, Args args):
        BasePromise() {
        visitResults([&](std::vector<std::future<void>>& res) {
            res.push_back(std::async(std::launch::async, [](F f, Args args){
                f(std::forward<Args>(args));
            },
            std::forward<F>(f),
            std::forward<Args>(args)));
        });
    }

    virtual ~Promise() {
    }
};

template<typename F, typename... Args>
Promise<F> async(F f, Args... args) {
    return Promise<F>(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace exp

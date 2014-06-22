#include "../lib/ytest/ytest.hpp"
#include <type_traits>
#include "../cu.hpp"

#include <string>
#include <list>

static int testFunc(int a, int b) {
    return a + b;
}

struct TestStruct {
    int i;
    void set(int v) { i = v; }
    int get() const { return i; }
};

yTestPackage pkgFunctionTraits([]{
    describe("function traits", []{

        it("works with free functions", []{
            testFunc(5, 5);
            typedef cu::tmp::function_traits<decltype(testFunc)> f_type;
            static_assert(std::is_same<f_type::result_type, int>::value, "return value is not int");
            static_assert(f_type::arity == 2, "function arity is wrong");
            static_assert(std::is_same<f_type::argument<0>::type, int>::value, "first parameter is not int");
            static_assert(std::is_same<f_type::argument<1>::type, int>::value, "second parameter is not int");
        });

        it("works with member functions", []{
            typedef cu::tmp::function_traits<decltype(&TestStruct::set)> s_type;
            typedef cu::tmp::function_traits<decltype(&TestStruct::get)> g_type;
            static_assert(std::is_void<s_type::result_type>::value, "member is not void");
            static_assert(std::is_same<g_type::result_type, int>::value, "member doesn't return int");
            static_assert(s_type::arity == 1, "function arity is wrong");
            static_assert(g_type::arity == 0, "function arity is wrong");
            static_assert(std::is_same<s_type::argument<0>::type, int>::value, "first parameter is not int");
        });

        it("works with lambdas", []{
            std::function<int(float, float)> lambda = [](float a, float b) -> int { return a + b; };
            typedef cu::tmp::functor_traits<decltype(lambda)> l_type;
            static_assert(std::is_same<l_type::result_type, int>::value, "return value is not int");
            static_assert(l_type::arity == 2, "function arity is wrong");
            static_assert(std::is_same<l_type::argument<0>::type, float>::value, "first parameter is not float");
            static_assert(std::is_same<l_type::argument<1>::type, float>::value, "second parameter is not float");
        });

    });
});

class Testi {
public:
    void wat() {
    }
};

yTestPackage tmp([]{
    describe("template metaprogramming helpers", []{

        it("can take nth parameter", [=]{
            auto x = cu::tmp::nth(1, "hello", "waat", "world");
            Assert().isEqual(x, "waat");
        });

        it("can take first argument", [=]{
            auto x = cu::tmp::head("hello", 42, "world");
            Assert().isEqual(x, "hello");
        });

    });
});

template<typename T,
         cu::tmp::enable_if<std::is_scalar<T>>* = nullptr>
int something(T) {
    return 1;
}

template<typename T,
         cu::tmp::enable_if<cu::tmp::has_random_access_iterators<T>>* = nullptr>
int something(T) {
    return 2;
}

template<typename T,
         cu::tmp::enable_if<cu::tmp::has_bidirectional_iterators<T>>* = nullptr>
int something(T) {
    return 3;
}

template<typename T,
         cu::tmp::enable_if<cu::tmp::has_forward_iterators<T>>* = nullptr>
int something(T) {
    return 4;
}

yTestPackage traitHelpers([]{
    describe("", []{
        it("", [] {
            std::vector<int> raitest;
            std::list<int> bidirtest;
            std::forward_list<int> forwtest;
            Assert().isEqual(something(5), 1)
                    .isEqual(something(raitest), 2)
                    .isEqual(something(bidirtest), 3)
                    .isEqual(something(forwtest), 4);
        });
    });
});

#include "../lib/ytest/ytest.hpp"
#include "../utils/experimental/mixin.hpp"

class A {
public:
    A& operator()() { return *this; }
    int method42() { return 42; }
};

class B {
    int x;
public:
    B& operator()() { return *this; }
    int get() const { return x; }
    void set(int v) { x = v; }
};

yTestPackage experimentals([]{
    describe("Mixin", []{
        it("somewhat works", []{
            cd::experimental::Mixin<A, B> mixin;
            mixin.comp<B>().set(10);
            auto re1 = mixin.comp<A>().method42(),
                 re2 = mixin.comp<B>().get();
            Assert().isEqual(re1, 42)
                    .isEqual(re2, 10);
        });
    });
});

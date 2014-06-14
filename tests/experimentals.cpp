#include "../lib/ytest/ytest.hpp"
#include "../utils/experimental/mixin.hpp"
#include "../utils/experimental/async.hpp"
#include "../utils/functions.hpp"

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

class C : public cd::experimental::Component<C> {
public:
    bool foobar() { return true; }
};

struct SingleInterface : public cd::experimental::Mixin<A, B, C> {
    MixinInterface(A, method42, method42)
    MixinInterfaceVoid(B, set, set)
    MixinInterface(B, get, get)
    MixinInterface(C, foobar, foobar)
};

void printtaa(std::size_t id, std::string msg) {
    static std::mutex tex;
    std::lock_guard<std::mutex> l(tex);
    std::cout << id << ": " << msg << std::endl;
}

yTestPackage experimentals([]{
    describe("async", []{
        // using namespace ce;
        // for (std::size_t i = 0; i < 50000; ++i) {
        //     async([](int i){
        //         printtaa(i, "ekassa");
        //         async([](int i){
        //             printtaa(i, "tokassa");
        //         }, i);
        //         async([](int i){
        //             printtaa(i, "toka rinnakkainen");
        //             async([](int i){
        //                 printtaa(i, "kolmas rinnakkainen");
        //             }, i);
        //         }, i);
        //     }, i);
        // }
        // std::cout << "launched" << std::endl;
    });

    describe("Mixin", []{

        it("somewhat works", []{
            cd::experimental::Mixin<A, B, C> mixin;
            mixin.component<B>().set(10);
            auto re1 = mixin.component<A>().method42(),
                 re2 = mixin.component<B>().get();
            auto re3 = mixin.component<C>().foobar();
            Assert().isEqual(re1, 42)
                    .isEqual(re2, 10)
                    .isEqual(re3, true);
        });

        it("works with a single interface ", []{
            SingleInterface si;
            si.set(10);
            Assert().isEqual(si.method42(), 42)
                    .isEqual(si.get(), 10)
                    .isEqual(si.foobar(), true);
        });

    });
});

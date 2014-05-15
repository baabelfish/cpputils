#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <map>
#include <string>

yTestPackage function_module([]{
    describe("function module", []{
        std::vector<int> x{1,2,3,4};

        it("has times", []{
            int x = 0;
            cu::times(5, [&]() { x += 1; });
            Assert().isEqual(x, 5);

            x = 0;
            cu::times(5, [&](int y) { x += y; }, 2);
            Assert().isEqual(x, 10);
        });

        it("has uniqueId", []{
            auto ui = cu::uniqueId(),
                 ui2 = cu::uniqueId();
            auto ui3 = cu::uniqueId("foo_");
            auto ui4 = cu::uniqueId(L"bar_");
            Assert().isEqual(ui, 0)
                    .isEqual(ui2, 1)
                    .isEqual(ui3, "foo_2")
                    .isEqual(ui4, L"bar_3");
        });

        it("has wait", []{ cu::wait(10); });

        it("has once", []{
            auto f = cu::once([](int a, int b) { return a + b; });
            auto v1 = f(8, 5),
                 v2 = f(10, 10);
            Assert().isEqual(v1, 13);
            Assert().isEqual(v1, v2);
        });

        it("has wrap", []{
            auto f = cu::wrap(15, [](int a, int b) { return a + b; });
            auto v = f(8);
            Assert().isEqual(v, 23);
        });

        it("has defer", []{
            int test = 0;
            {
                auto handle = cu::defer([&]() { test += 1; });
                test += 1;
                handle.get();
            }
            Assert().isEqual(test, 2);
        });

        it("has delay", []{
            int test = 0;
            {
                auto handle = cu::delay(100, [&]() { test = 2; });
                test = 1;
            }
            Assert().isEqual(test, 2);
        });

        it("has identity", []{
            Assert().isEqual(cu::identity(5), 5)
                    .isEqual(cu::identity(5, 10, 2), 5);
        });

        it("can tap", []{
            int x = 5;
            auto inc = [](int i) { return ++i; };
            auto dou = [](int i) { return i * 2; };
            x = inc(cu::tap(dou(x), [](int& i) { i = i - 1; }));
            Assert().isEqual(x, 10);
        });

        it("can extract keys and values", []{
            std::map<int, std::string> m{
                {1, "foo"},
                {2, "bar"}
            };
            auto mk = cu::keys(m);
            auto mv = cu::values(m);

            Assert()
                .isEqual(mk, {1, 2})
                .isEqual(mv, {"foo", "bar"});
        });

        it("can do after", []{
            int x = 0;
            auto aft = cu::after(3, [&]{ x = 5; });
            Assert().isEqual(x, 0);
            aft();
            aft();
            Assert().isEqual(x, 0);
            aft();
            Assert().isEqual(x, 5);
        });

        it("can do after with parameters", []{
            int x = 0;
            auto aft = cu::after(3, [&](int a){ x = a; }, 10);
            Assert().isEqual(x, 0);
            aft();
            aft();
            Assert().isEqual(x, 0);
            aft();
            Assert().isEqual(x, 10);
        });

    });
});

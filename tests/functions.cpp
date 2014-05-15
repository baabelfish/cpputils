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

        it("has delay", []{
            int test = 0;
            {
                auto hadle = cu::delay(100, [&]() { test = 2; });
                test = 1;
            }
            Assert().isFalse(2);
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

    });
});

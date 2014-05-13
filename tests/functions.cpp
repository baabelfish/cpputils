#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <map>
#include <string>

yTestPackage functions([]{
    describe("function module", []{
        std::vector<int> x{1,2,3,4};

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

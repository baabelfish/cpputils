#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <map>
#include <string>

struct OtherType {
    int old_value;
    int new_value;
};

yTestPackage containers([]{
    describe("container module", []{
        std::vector<int> x{1,2,3,4};

        it("inplace map", [=]{
            auto nx = cu::map(x, [](int x) { return x * 2; });
            Assert().isEqual(nx, {2,4,6,8});
        });

        it("type changing map", [=]{
            auto nx = cu::map<std::vector<OtherType>>(x, [](int x) {
                return OtherType{ x, x * 2 };
            });
            Assert()
                .isEqual(nx.size(), x.size())
                .isEqual(nx[3].new_value, 8);
        });

        it("can manipulate containers", []{
            std::vector<int> x{1,2,3,4};
            cu::each(x, [](int& i) { i = i * 2; });
            Assert().isEqual(x, {2,4,6,8});
        });

        it("can filter", [=]{
            auto nx = cu::filter(x, [](int i) { return i > 2; });
            Assert().isEqual(nx, {3, 4});
        });

        it("can access limits", [=]{
            auto first = cu::first(x);
            auto last = cu::last(x);
            Assert()
                .isEqual(first, x[0])
                .isEqual(last, x[x.size() - 1]);
        });

        it("can fold", [=]{
            auto val = cu::fold(std::vector<int>{}, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 0);
            val = cu::fold(std::vector<int>{1}, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 1);
            val = cu::fold(x, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 10);
        });

        it("can rfold", [=]{
            auto val = cu::rfold(std::vector<int>{}, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 0);
            val = cu::rfold(std::vector<int>{1}, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 1);
            val = cu::rfold(x, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val, 10);
        });

        it("can do otp", [=]{
            bool p_all = cu::all(x, [](int i) { return i < 5; }),
                 p_any = cu::any(x, [](int i) { return i == 2; }),
                 p_none = cu::none(x, [](int i) { return i > 4; });

            Assert()
                .isTrue(p_all)
                .isTrue(p_any)
                .isTrue(p_none);
        });
    });
});

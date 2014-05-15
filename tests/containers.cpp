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

        it("can sort", []{
            Assert().isEqual(cu::sort(std::vector<int>{1,4,2,3}), {1,2,3,4});
        });

        it("can sort with a function", []{
            Assert().isEqual(cu::sort(std::vector<int>{1,4,2,3}, [](int a, int b) { return a > b; }), {4,3,2,1});
        });

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

        it("has find", []{
            std::vector<int> x{1,3,5,6,4,2};
            Assert().isEqual(*cu::find(x, [](int i) { return i > 2; }), 3);
        });

        it("has findLast", []{
            std::vector<int> x{1,3,5,6,4,2};
            Assert().isEqual(*cu::findLast(x, [](int i) { return i > 2; }), 4);
        });

        it("has unique", []{
            std::vector<int> x{1,3,5,2,3,2,4};
            Assert().isEqual(cu::unique(x), {1,3,5,2,4});
        });

        it("has contains", []{
            Assert().isTrue(cu::contains("foobar", "foo"))
                    .isTrue(cu::contains(L"foobar", L"foo"))
                    .isTrue(cu::contains(std::map<int, int>{ {1, 2}, {3, 4} }, 1))
                    .isTrue(cu::contains(std::set<int>{ 1, 2, 3, 4 }, 1))
                    .isTrue(cu::contains(std::vector<int>{ 1, 2, 3, 4 }, 1))
                    .isTrue(cu::contains(std::string("foobar"), "foo"));
        });

        it("has at", []{
            std::vector<int> x{1,2,3,4};
            auto res = cu::at(x, 1, 2);
            Assert().isEqual(res, {2,3});
        });

        it("can manipulate containers", []{
            std::vector<int> x{1,2,3,4};
            cu::each(x, [](int& i) { i = i * 2; });
            Assert().isEqual(x, {2,4,6,8});
        });

        it("can test equality of ranges", [=]{
            Assert().isTrue(cu::areEqual(std::vector<int>{1,2,3}, std::vector<int>{1,2,3}))
                    .isTrue(cu::areEqual(std::vector<int>{}, std::vector<int>{}))
                    .isTrue(cu::areEqual({1}, {1}))
                    .isFalse(cu::areEqual(std::vector<int>{1,2,3}, std::vector<int>{1,2,3,4}))
                    .isFalse(cu::areEqual(std::vector<int>{1,3,3}, std::vector<int>{1,2,3}));
        });

        it("can test equality of ranges with custom a function", [=]{
            Assert().isTrue(cu::areEqual({1}, {"1"}, [](int a, std::string b) {
                return std::to_string(a) == b;
            }));
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
            auto v = cu::size(std::vector<int>{1,2,3});
            Assert().isEqual(v, 3);
        });

        it("can fold", [=]{
            auto val1 = cu::fold(std::vector<int>{}, [](int acc, int i) { return acc + i; }),
                 val2 = cu::fold(std::vector<int>{1}, [](int acc, int i) { return acc + i; }),
                 val3 = cu::fold(x, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val1, 0);
            Assert().isEqual(val2, 1);
            Assert().isEqual(val3, 10);
        });

        it("can rfold", [=]{
            auto val1 = cu::rfold(std::vector<int>{}, [](int acc, int i) { return acc + i; }),
                 val2 = cu::rfold(std::vector<int>{1}, [](int acc, int i) { return acc + i; }),
                 val3 = cu::rfold(x, [](int acc, int i) { return acc + i; });
            Assert().isEqual(val1, 0);
            Assert().isEqual(val2, 1);
            Assert().isEqual(val3, 10);
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

#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <deque>
#include <list>
#include <forward_list>
#include <cmath>
#include <map>
#include <string>

struct OtherType {
    int old_value;
    int new_value;
};

yTestPackage containers([]{
    describe("container module", []{
        std::vector<int> x{1,2,3,4};

        it("can initialize containers", []{
            Assert().isEqual(std::set<int>{1,2,3}, cu::set(1,2,3));
            Assert().isEqual(std::multiset<int>{1,2,3}, cu::multiset(1,2,3));
            Assert().isEqual(std::unordered_set<int>{1,2,3}, cu::uset(1,2,3));
            Assert().isEqual(std::unordered_multiset<int>{1,2,3}, cu::multiuset(1,2,3));
            Assert().isEqual(std::forward_list<int>{1,2,3}, cu::flist(1,2,3));
            Assert().isEqual(std::list<int>{1,2,3}, cu::list(1,2,3));
            Assert().isEqual(std::deque<int>{1,2,3}, cu::deque(1,2,3));
            Assert().isEqual(std::vector<int>{1,2,3}, cu::vec(1,2,3));
        });

        it("can convert containers", []{
            Assert().isEqual(std::unordered_set<int>{1,2,3}, cu::uset(std::vector<int>{1,2,3}));
            Assert().isEqual(std::forward_list<int>{1,2,3}, cu::flist(std::vector<int>{1,2,3}));
            Assert().isEqual(cu::flist(std::vector<int>{1,2,3}), std::forward_list<int>{1,2,3});
            Assert().isEqual(std::vector<int>{1,2,3}, cu::vec(std::list<int>{1,2,3}));
        });

        it("has prepend", []{
            std::deque<int> x{1,2};
            std::list<int> y{3,4};

            std::forward_list<int> x2{1,2};
            std::list<int> y2{3,4};

            auto z = cu::prepend(x, y);
            auto z2 = cu::prepend(x2, y2);

            Assert().isEqual(z, {3,4,1,2});
            Assert().isEqual(z2, {3,4,1,2});
        });

        it("can clear", []{
            auto x = cu::vec(1,2,3);
            std::stack<int> y;
            std::queue<int> z;
            y.push(1);
            y.push(2);
            y.push(3);
            z.push(1);
            cu::clear(x);
            cu::clear(y);
            cu::clear(z);
            Assert().isTrue(x.empty())
                    .isTrue(y.empty())
                    .isTrue(z.empty());
        });

        it("has mapcat", []{
            std::vector<int> x{3,2,1},
                             y{6,5,4},
                             z{9,8,7};
            auto w = cu::mapcat(&cu::sort<decltype(x)>, x, y, z);
            Assert().isEqual(w, {1,2,3,4,5,6,7,8,9});
        });

        it("has concat", []{
            std::vector<int> x{1,2};
            std::list<int> y{3,4};
            std::deque<int> z{5,6};
            auto w = cu::concat(x, y, z);
            Assert().isEqual(w, {1,2,3,4,5,6});
        });

        it("has reverse", []{
            std::vector<int> x{1,2,3};
            Assert().isEqual(cu::reverse(x), {3,2,1});
        });

        it("can groupBy", []{
            std::vector<float> x{1.3, 1.5, 1.2, 2.3, 2.5, 3.7};
            auto grouped = cu::groupBy<int>(x, [](float i) -> int {
                return std::floor(i);
            });
            Assert().isEqual(grouped.size(), 3);
            Assert().isEqual(grouped[1].size(), 3);
            Assert().isEqual(grouped[2].size(), 2);
            Assert().isEqual(grouped[3].size(), 1);
        });

        it("can find min and max elements", []{
            std::vector<int> x{1,2,3,4,3,0,2};
            auto minmax = cu::minmax(x);
            auto min = cu::min(x),
                 max = cu::max(x);
            Assert().isEqual(minmax.min, 0);
            Assert().isEqual(minmax.max, 4);
            Assert().isEqual(min, 0);
            Assert().isEqual(max, 4);
        });

        it("can construct a range", []{
            Assert().isEqual(cu::range(0, 10), {0,1,2,3,4,5,6,7,8,9});
        });

        it("can construct a range with generator", []{
            Assert().isEqual(cu::range(0, [](int i) -> bool { return i < 6; }, [](int i) { return ++i; }), {0,1,2,3,4,5});
        });

        it("can get frequencies", []{
            auto freqs = cu::frequencies({1,1,1,2,2,3});
            Assert().isEqual(freqs[1], 3)
                    .isEqual(freqs[2], 2);
        });

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

        it("has filter", [=]{
            auto nx = cu::reject(x, [](int i) { return i > 2; });
            Assert().isEqual(nx, {1, 2});
        });

        it("has reject", [=]{
            auto nx = cu::filter(x, [](int i) { return i > 2; });
            Assert().isEqual(nx, {3, 4});
        });

        it("can access limits", [=]{
            auto first = cu::first(x);
            auto last = cu::last(x);
            Assert().isEqual(first, x[0])
                    .isEqual(last, x[x.size() - 1]);
        });

        it("has size", [=]{
            auto v = cu::size(std::vector<int>{1,2,3});
            Assert().isEqual(v, 3);
        });

        it("has fold", [=]{
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

            Assert().isTrue(p_all)
                    .isTrue(p_any)
                    .isTrue(p_none);
        });
    });
});

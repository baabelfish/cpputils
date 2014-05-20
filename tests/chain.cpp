#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <list>

yTestPackage chain([]{
    describe("chaining module", []{
        std::vector<int> X{1,2,3};

        it("can construct containers", []{
            std::forward_list<int> flist = cu::flist(1,2,3);
            std::list<int> list = cu::list(1,2,3);
            std::vector<int> vec = cu::vec(1,2,3);
            std::deque<int> deque = cu::deque(1,2,3);
        });

        it("can construct chains", []{
            auto chain = _({1,2,3});
            auto chain2 = _(std::forward_list<int>{1,2,3});
            auto chain3 = _(std::set<int>{1,2,3});
            auto chain4 = _(std::map<int, int>{{1,2}, {3,4}});
            Assert().isEqual(chain.value(), {1,2,3});
            Assert().isEqual(chain2.value(), {1,2,3});
            Assert().isEqual(chain3.value(), {1,2,3});
            Assert().isEqual(chain4.value(), {{1,2}, {3,4}});
        });

        it("can be copied", [=]{
            auto l = _(X);
            auto r = l.copy();
            Assert().isEqual(r.value(), {1,2,3})
                    .isEqual(l.value(), {1,2,3});
            l = _(X).map([](int i) { return i + 1; });
            auto l1 = l.copy().map([](int i) { return i * 2; }).value();
            auto l2 = l.copy().map([](int i) { return i * 3; }).value();
            Assert().isEqual(l1, {4, 6, 8})
                    .isEqual(l2, {6, 9, 12});
        });

        it("uses move semantics", [=]{
            auto l = _(X);
            Assert().isEqual(l.value(), {1,2,3});
            auto r = l.value();
            Assert().isEqual(r, {1,2,3})
                    .isEqual(l.value(), {});
        });

        it("can chain multiple functions", [=]{
            auto l = _(X).map([](int i) { return i + 1; })
                         .map([](int i) { return i * 2; })
                         .value();
            Assert().isEqual(l, {4, 6, 8});
        });

        it("can chain multiple functions", [=]{
            auto l = _(X).map([](int i) { return i + 1; })
                         .map([](int i) { return i * 2; })
                         .value();
            Assert().isEqual(l, {4, 6, 8});
        });

        it("works with all chainables", [=]{
            auto filtered = _(X).filter([](int i) { return i < 3; }).value(),
                 mapped = _(X).map([](int i) { return i * 2; }).value();
            Assert().isEqual(filtered, {1,2})
                    .isEqual(mapped, {2,4,6});
        });

        it("works with all returning with function as parameter", [=]{
            bool any = _(X).any([](int i) { return i == 2; }),
                 none = _(X).none([](int i) { return i == 5; }),
                 all = _(X).all([](int i) { return i > 0 && i < 4; });
            Assert().isTrue(any)
                    .isTrue(none)
                    .isTrue(all);
        });

        it("works with custom chainables", [=]{
            auto cs = _(X).custom([](decltype(X)&& con) { return con; }).value();
            Assert().isEqual(cs, {1,2,3});
        });

        it("works with all returning without parameters", [=]{
            auto size = _(X).size();
            Assert().isEqual(size, 3);
        });

        it("can do a real world example", []{
            std::vector<int> example{1,1,2,3,4,2,3,4,5,6,7,1,2,8,9};
            auto re = _(example).filter([](int i) { return i < 7; })
                                .sort()
                                .unique()
                                .map([](int i) { return i * 2; });
            Assert().isEqual(re.value(), {2, 4, 6, 8, 10, 12});
        });

        it("works with README.md example", [=]{
            auto chain = _({1,2,3,4}).reject([](int i) { return i < 2; })
                                     .reverse()
                                     .concat(cu::list(1,2))
                                     .map([](int i) { return i * 2; })
                                     .concat(cu::vec(3));
            Assert().isEqual(chain.value(), { 8, 6, 4, 2, 4, 3 });
            Assert().isTrue(chain.contains(8, 6, 3));
        });

    });
});

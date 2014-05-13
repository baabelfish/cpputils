#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

struct TestS {
    int x;
    std::string y;
    bool operator!=(const TestS& other) const { return other.x != x || other.y != y; }
};

yTestPackage chain([]{
    describe("chaining module", []{
        std::vector<int> X{1,2,3};

        it("has value_type", []{
            cu::Chain<TestS>::value_type l{1, "test"};
            Assert().isEqual(l, TestS{1, "test"});
        });

        it("can return endval", []{
            auto l = _(5);
            Assert().isEqual(l.value(), 5);
        });

        it("can copy", [=]{
            auto l = _(X);
            auto r = l.copy();
            Assert()
                .isEqual(r.value(), {1,2,3})
                .isEqual(l.value(), {1,2,3});
        });

        it("uses move semantics", [=]{
            auto l = _(X);
            Assert().isEqual(l.value(), {1,2,3});
            auto r = l.value();
            Assert()
                .isEqual(r, {1,2,3})
                .isEqual(l.value(), {});
        });

        it("works with basic use case", [=]{
            auto l = _(X);
            auto r = l.value();
            Assert()
                .isEqual(r, {1,2,3})
                .isEqual(l.value(), {});
        });

        it("can chain multiple functions", [=]{
            auto l = _(X).map([](int i) { return i + 1; })
                         .map([](int i) { return i * 2; })
                         .value();
            Assert()
                .isEqual(l, {4, 6, 8});
        });

        it("can chain multiple functions", [=]{
            auto l = _(X).map([](int i) { return i + 1; })
                         .map([](int i) { return i * 2; })
                         .value();
            Assert()
                .isEqual(l, {4, 6, 8});
        });

        it("is somewhat lazy", [=]{
            auto l = _(X).map([](int i) { return i + 1; });

            auto l1 = l.copy()
                        .map([](int i) { return i * 2; })
                        .value();

            auto l2 = l.copy().map([](int i) { return i * 3; })
                              .value();

            Assert()
                .isEqual(l1, {4, 6, 8})
                .isEqual(l2, {6, 9, 12});
        });

        it("works with all chainables", [=]{
            auto filtered = _(X)
                            .filter([](int i) { return i < 3; })
                            .value(),
                 mapped = _(X)
                            .map([](int i) { return i * 2; })
                            .value();
            Assert()
                .isEqual(filtered, {1,2})
                .isEqual(mapped, {2,4,6});
        });

        it("works with all returning with function as parameter", [=]{
            bool any = _(X).any([](int i) { return i == 2; }),
                 none = _(std::vector<int>{1,2,3}).none([](int i) { return i == 5; }),
                 all = _(std::vector<int>{1,2,3}).all([](int i) { return i > 0 && i < 4; });
            Assert()
                .isTrue(any)
                .isTrue(none)
                .isTrue(all);
        });

        it("works with custom chainables", [=]{
            auto cs = _(X)
                        .custom([](decltype(X)&& con) { return con; })
                        .value();
            Assert().isEqual(cs, {1,2,3});
        });

        it("works with all returning without parameters", [=]{
            auto size = _(X).size();
            Assert().isEqual(size, 3);
        });

    });
});

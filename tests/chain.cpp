#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

struct TestS {
    int x;
    std::string y;
    bool operator!=(const TestS& other) const { return other.x != x || other.y != y; }
};

yTestPackage chain([]{
    describe("chaining module", []{

        it("has value_type", []{
            Chain<TestS>::value_type l{1, "test"};
            Assert().isEqual(l, TestS{1, "test"});
        });

        it("can return endval", []{
            auto l = _(5);
            Assert()
                .isEqual(l.value(), 5);
        });

        it("uses move semantics", []{
            auto l = _(std::vector<int>{1,2,3});
            Assert().isEqual(l.value(), {1,2,3});

            auto r = l.value();
            Assert()
                .isEqual(r, {1,2,3})
                .isEqual(l.value(), {});
        });

    });
});

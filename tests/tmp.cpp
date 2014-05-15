#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <string>

yTestPackage tmp([]{
    describe("template metaprogramming helpers", []{

        it("can check if types are same", [=]{
            bool x = cu::tmp::isSame<int, int>::value,
                 y = cu::tmp::isSame<int, unsigned>::value;
            Assert().isTrue(x)
                    .isFalse(y);
        });

        it("can take nth parameter", [=]{
            auto x = cu::tmp::nth(1, "hello", "waat", "world");
            Assert().isEqual(x, "waat");
        });

        it("can take first argument", [=]{
            auto x = cu::tmp::head("hello", 42, "world");
            Assert().isEqual(x, "hello");
        });

    });
});

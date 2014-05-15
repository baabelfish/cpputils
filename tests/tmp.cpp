#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <string>

yTestPackage tmp([]{
    describe("template metaprogramming helpers", []{

        it("can take first argument", [=]{
            auto x = cu::tmp::head("hello", 42, "world");
            Assert().isEqual(x, "hello");
        });

    });
});

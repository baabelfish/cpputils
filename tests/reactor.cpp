#include "../lib/ytest/ytest.hpp"
#include "../utils/experimental/react.hpp"

using cd::experimental::React;

struct Teststruct {
    int a;
    int b;
};

yTestPackage reactor([]{
    describe("React", []{

        it("", []{
            auto a = React<int>(0);
            auto b = React<int>(0);
            auto c = React<int>(0);
            auto d = React<Teststruct>({0, 0});

            a.after([&](int x) { b = x * x; });
            b.after([&](int x) { c = x * 2; });
            d.after([&](Teststruct x) { c += x.a + x.b; });

            a = 2;
            d.visit([](Teststruct& x) {
                x.a = 1;
                x.b = 2;
            });

            Assert().isEqual(*c, 11);
        });

    });
});

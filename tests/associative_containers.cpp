#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <map>
#include <string>

yTestPackage AssociativeContainers([]{
    describe("Basic operations", []{
        it("can extract keys and values", []{
            std::map<int, std::string> m{
                {1, "foo"},
                {2, "bar"}
            };
            auto mk = cu::keys(m);
            auto mv = cu::values(m);

            Assert().isEqual(mk, {1, 2})
                    .isEqual(mv, {"foo", "bar"});
        });

        it("has omit", []{
            std::map<int, std::string> m{
                {1, "foo"},
                {2, "bar"},
                {3, "test"}
            };
            m = cu::omit(m, 2, 3);
            Assert().isEqual(m.size(), 1);
        });

        it("has mapValues", []{
            std::map<int, std::string> m{
                {1, "foo"},
                {2, "bar"},
                {3, "test"}
            };
            m = cu::mapValues(m, [](std::string s) { return s + "_"; });
            auto mv = cu::values(m);
            Assert().isEqual(mv, {"foo_", "bar_", "test_"});
        });

    });

    describe("zip", []{
        it("can zipMap", []{
            std::vector<int> keys{1,2,3,4};
            std::vector<std::string> values{"foo", "bar", "test"};
            auto zm = cu::zipMap(keys, values);
            Assert().isEqual(zm.size(), 3);
            Assert().isEqual(zm[1], "foo")
                    .isEqual(zm[2], "bar")
                    .isEqual(zm[3], "test");
        });

        it("can zipUnorderedMultiMap", []{
            std::vector<int> keys{1,1,2,2};
            std::vector<std::string> values{"foo", "bar", "test", "too", "many"};
            auto zm = cu::zipUnorderedMultiMap(keys, values);
            auto range = cu::values<std::string>(zm.equal_range(1));
            Assert().isEqual(zm.size(), 4);
            Assert().isEqual(cu::values<std::string>(zm.equal_range(1)), {"bar", "foo"});
        });
    });
});

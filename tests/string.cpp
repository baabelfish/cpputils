#include "../lib/ytest/ytest.hpp"
#include "../cu.hpp"

#include <string>

yTestPackage strings([]{

    describe("levenshtein distance", []{
        it("can calculate it", []{
            std::string a = "abbreviation";
            Assert().isEqual(cs::levenshteinDistance(a, std::string("abbreviation")), 0);
            Assert().isEqual(cs::levenshteinDistance(a, std::string("bbbreviation")), 1);
            Assert().isEqual(cs::levenshteinDistance(a, std::string("abbrevation")), 1);
            Assert().isEqual(cs::levenshteinDistance(a, std::string("bbbrevationz")), 3);
            Assert().isEqual(cs::levenshteinDistance(a, std::string("")), 12);
        });
    });

    describe("string module", []{

        it("can join", []{
            Assert().isEqual(cs::join(cu::vec<std::string>("foo", "bar"), ""), "foobar");
            Assert().isEqual(cs::join(cu::vec<std::string>("foo", "bar"), " "), "foo bar");
            Assert().isEqual(cs::join(cu::vec<std::string>("foo", "bar", "foo"), ", "), "foo, bar, foo");
        });

        it("can split", []{
            std::string words = "first second third";
            auto re = cs::split(words, " ");
            Assert().isEqual(re, { "first", "second", "third" });
        });

        it("can trim", []{
            Assert().isEqual(cs::trim(std::string("  foo bar   ")), std::string("foo bar"));
            Assert().isEqual(cs::trim(std::wstring(L"foobar ")), std::wstring(L"foobar"));
            Assert().isEqual(cs::trim(std::u16string(u" foobar")), std::u16string(u"foobar"));
            Assert().isEqual(cs::trim(std::u32string(U"foobar")), std::u32string(U"foobar"));
        });

        it("has beginsWith and endsWith", []{
            Assert().isTrue(cs::beginsWith(std::string("foobar"), std::string("foo")));
            Assert().isTrue(cs::endsWith(std::string("foobar"), std::string("bar")));
            Assert().isFalse(cs::endsWith(std::string("foobarfoo"), std::string("bar")));
        });

        it("can uppercase strings", []{
            Assert().isEqual(cs::toUpperCase("FooBaR"), "FOOBAR");
            Assert().isEqual(cs::toUpperCase(L"FooBaR"), L"FOOBAR");
            Assert().isEqual(cs::toUpperCase(u"FooBaR"), u"FOOBAR");
            Assert().isEqual(cs::toUpperCase(U"FooBaR"), U"FOOBAR");
        });

        it("can lowercase strings", []{
            Assert().isEqual(cs::toLowerCase("FooBaR"), "foobar");
            Assert().isEqual(cs::toLowerCase(L"FooBaR"), L"foobar");
            Assert().isEqual(cs::toLowerCase(u"FooBaR"), u"foobar");
            Assert().isEqual(cs::toLowerCase(U"FooBaR"), U"foobar");
        });

    });
});

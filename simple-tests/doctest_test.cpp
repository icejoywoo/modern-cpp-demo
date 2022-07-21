#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "fakeit/doctest/fakeit.hpp"

struct SomeInterface {
    virtual int foo(int) = 0;
    virtual int bar(std::string) = 0;
};

TEST_CASE("test mock lib fakeit") {
    using namespace fakeit;
    Mock<SomeInterface> mock;

    When(Method(mock, foo)).Return(0);

    SomeInterface &i = mock.get();

    // Production code
    CHECK_EQ(0, i.foo(1));

    // Verify method mock.foo was invoked.
    Verify(Method(mock,foo));

    // Verify method mock.foo was invoked with specific arguments.
    Verify(Method(mock,foo).Using(1));
}

TEST_CASE("vectors can be sized and resized") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SUBCASE("adding to the vector increases it's size") {
        v.push_back(1);

        CHECK(v.size() == 6);
        CHECK(v.capacity() >= 6);
    }
    SUBCASE("reserving increases just the capacity") {
        v.reserve(6);

        CHECK_EQ(v.size(), 5);
        CHECK(v.capacity() >= 6);
    }
}

/**
 * CHECK宏是只检查并不会终止测试，REQUIRE宏则会终止测试和gtest里面的ASSET_XX宏类似，
 * 这两个宏也是平时做单测时用得最多的两个宏了。除此之外，还有更丰富的宏。
 * <LEVEL> is one of 3 possible: REQUIRE/CHECK/WARN.
 * <LEVEL>_EQ(left, right) - same as <LEVEL>(left == right)
 * <LEVEL>_NE(left, right) - same as <LEVEL>(left != right)
 * <LEVEL>_GT(left, right) - same as <LEVEL>(left > right)
 * <LEVEL>_LT(left, right) - same as <LEVEL>(left < right)
 * <LEVEL>_GE(left, right) - same as <LEVEL>(left >= right)
 * <LEVEL>_LE(left, right) - same as <LEVEL>(left <= right)
 * <LEVEL>_UNARY(expr) - same as <LEVEL>(expr)
 * <LEVEL>_UNARY_FALSE(expr) - same as <LEVEL>_FALSE(expr)
 *
 * 判断异常的宏
 * CHECK_THROWS_AS(func(), const std::exception&);
 * CHECK_THROWS_AS(func(), std::exception); // same as above
 * CHECK_THROWS_WITH(func(), "invalid operation!");
 * CHECK_THROWS_WITH_AS(func(), "invalid operation!", std::runtime_error);
 * <LEVEL>_NOTHROW(expression)
*/
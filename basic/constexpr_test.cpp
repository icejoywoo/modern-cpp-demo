#include "gtest/gtest.h"

#include <limits>

#define ASSERT_ARRAY_SIZE_AND_VALUE(array, size, initial_value) \
    ASSERT_EQ(size, sizeof array); \
    for (const auto& e : array) { \
        ASSERT_EQ(initial_value, e); \
    }

#define SIMPLE_PRINT(expr, format) \
    printf(#expr ": " #format "\n", expr)

TEST(ConstexprSuite, BasicTest) {
//    int x1 = 42;
//    constexpr int x2 = x1; // compile error
    const int x1 = 42;
    constexpr int x2 = x1;
    char buffer[x2] = {0};
    ASSERT_ARRAY_SIZE_AND_VALUE(buffer, x2, 0);
}

constexpr int square(int x) {
    return x * x;
}

TEST(ConstexprSuite, FunctionTest) {
    char buffer[square(5)] = {0};
    ASSERT_ARRAY_SIZE_AND_VALUE(buffer, square(5), 0);
}

// recursive constexpr to compute sum in compile phase
constexpr int sum(int x) {
    return x > 0 ? sum(x - 1) + x : 0;
}

TEST(ConstexprSuite, RecursiveFunctionTest) {
    ASSERT_EQ(5050, sum(100));
}

TEST(ConstexprSuite, LimitsTest) {
    SIMPLE_PRINT(std::numeric_limits<unsigned char>::max(), "%u");
    char buffer[std::numeric_limits<unsigned char>::max()] = {0};
    ASSERT_ARRAY_SIZE_AND_VALUE(buffer, std::numeric_limits<unsigned char>::max(), 0);
}

TEST(ConstexprSuite, SimpleStructTest) {
    struct X {
        int x1;
    };
    constexpr X x = { 1 };
    char buffer[x.x1] = {0};
    SIMPLE_PRINT(x.x1, "%d");
    ASSERT_ARRAY_SIZE_AND_VALUE(buffer, x.x1, 0);
}

TEST(ConstexprSuite, ConstructFunctionTest) {
    class X {
    public:
        constexpr X(): x1(5) {}
        constexpr X(int i): x1(i) {}
        constexpr int get() const {
            return x1;
        }
    private:
        int x1;
    };
    constexpr X x;
    SIMPLE_PRINT(x.get(), "%d");
    char buffer[x.get()] = {0};
    ASSERT_ARRAY_SIZE_AND_VALUE(buffer, x.get(), 0);
}

constexpr double sum(double x) {
    return x > 0 ? x + sum(x - 1) : 0;
}

TEST(ConstexprSuite, DoubleTest) {
    constexpr double s = sum(5.0);
    SIMPLE_PRINT(s, "%f");
}
#include "template/basics/max.hpp"
#include "template/basics/stack1.hpp"
#include "basic/common_util.h"

#include <gtest/gtest.h>

#include <deque>

TEST(TemplateBasicsTest, MaxTest) {
    int i = 42;
    SIMPLE_PRINT(::max(7, i), "%d");
    double f1 = 3.4;
    double f2 = -6.7;
    SIMPLE_PRINT(::max(f1, f2), "%f");

#ifndef linux
    // 下面这段代码在gcc下会segment fault，原因暂时不知道
    SIMPLE_PRINT(::max2(f1, i), "%f");
    SIMPLE_PRINT((::max3<double, double, int>(f1, i)), "%f");
    SIMPLE_PRINT(::max3<double>(f1, i), "%f");
#endif

    std::string s1 = "mathematics";
    std::string s2 = "math";
    SIMPLE_PRINT(::max(s1, s2).c_str(), "%s");
}

TEST(TemplateBasicsTest, StackTest) {
    Stack<int> intStack;
    intStack.push(7);
    ASSERT_EQ(7, intStack.top());
    ASSERT_FALSE(intStack.empty());
    intStack.pop();
    ASSERT_TRUE(intStack.empty());
    try {
        intStack.pop();
    } catch (std::out_of_range const& ex) {
        printf("exception: %s\n", ex.what());
    }

    Stack<std::string> stringStack;
    stringStack.push("hello");
    ASSERT_EQ("hello", stringStack.top());
    ASSERT_FALSE(stringStack.empty());
    stringStack.pop();
    ASSERT_TRUE(stringStack.empty());
    try {
        stringStack.pop();
    } catch (std::out_of_range const& ex) {
        printf("exception: %s\n", ex.what());
    }
}

template <typename T, int VAL>
T addValue(T const& x) {
    return x + VAL;
}

TEST(TemplateBasicsTest, FunctionTest) {
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::vector<int> dest(5/*size*/);
    std::transform(source.begin(), source.end(), dest.begin(), addValue<int, 5>);
//    std::transform(source.begin(), source.end(), dest.begin(), (int(*)(int const&)) addValue<int, 5>);
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(i + 1 + 5, dest[i]);
    }
}

template<uint64_t N>
uint64_t fib() {
    return fib<N-1>() + fib<N-2>();
}

template<>
uint64_t fib<0>() {
    return 0;
}

template<>
uint64_t fib<1>() {
    return 1;
}

uint64_t simple_fib(long n) {
    if (n <= 1) {
        return n;
    }
    uint64_t a = 0;
    uint64_t b = 1;
    uint64_t tmp = b;
    for (int i = 2; i <= n; i++) {
        b = a + b;
        a = tmp;
        tmp = b;
    }
    return b;
}

TEST(TemplateBasicsTest, RecusiveTest) {
    ASSERT_EQ(simple_fib(2), fib<2>());
    ASSERT_EQ(simple_fib(10), fib<10>());
    // printf("%lu\n", fib<100>());  // 这个无法打印出来
    printf("%lu\n", simple_fib(100));
}

// c++ 14
template <typename T>
T sum(T arg) {
    return arg;
}

template <typename T1, typename... Args>
auto sum(T1 arg1, Args... args) {
    return arg1 + sum(args...);
}

// c++ 17
// 折叠表达式
template <typename... Args>
auto sum2(Args... args) {
    // 一元向右折叠
    return (args + ...);
}

template <typename... Args>
auto sum3(Args... args) {
    // 一元向左折叠
    return (... + args);
}

TEST(TemplateBasicsTest, RecusiveVarArgsTest) {
    ASSERT_EQ(sum(1, 2, 3), 6);
    // float-pointing macros
    EXPECT_FLOAT_EQ(sum(1, 5.0, 11.7), 17.7);
    EXPECT_FLOAT_EQ(sum(1, 5.0f, 11.7), 17.7);
    EXPECT_FLOAT_EQ(sum(1, 5.0, 11.7f), 17.7);

    ASSERT_EQ(sum2(1, 2, 3), 6);
    // float-pointing macros
    EXPECT_FLOAT_EQ(sum2(1, 5.0, 11.7), 17.7);
    EXPECT_FLOAT_EQ(sum2(1, 5.0f, 11.7), 17.7);
    EXPECT_FLOAT_EQ(sum2(1, 5.0, 11.7f), 17.7);

    ASSERT_EQ("hello c++ world", sum3(std::string("hello"), " c++", " world"));
}

// c++ 17 using 包展开

template <typename T>
class Base {
public:
    Base() {}
    Base(T t) : t_(t) {}
private:
    T t_;
};

template <typename... Args>
class Derived : public Base<Args>...
{
public:
    using Base<Args>::Base...;
};

TEST(TemplateBasicsTest, UsingVarArgsTest) {
    Derived<int, std::string, bool> d1 = 11;
    Derived<int, std::string, bool> d2 = std::string("hello");
    Derived<int, std::string, bool> d3 = true;
    // 没办法访问成员变量 t_，这语法具体使用场景？
}

// c++ 14 变量模板
template <typename T>
constexpr T PI = static_cast<T>(3.1415926535897932385L);

TEST(TemplateBasicsTest, VarTemplateTest) {
    ASSERT_FLOAT_EQ(3.141593, PI<float>);
    ASSERT_FLOAT_EQ(3.141593, PI<double>);
    ASSERT_EQ(3, PI<int>);
    ASSERT_EQ(3, PI<long>);

    // c++14
    ASSERT_FALSE((std::is_same<int, std::size_t>::value));
    // c++17
    ASSERT_FALSE((std::is_same_v<int, std::size_t>));
}
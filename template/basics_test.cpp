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
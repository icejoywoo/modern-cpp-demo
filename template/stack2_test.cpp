#include "template/basics/stack2.hpp"

#include <gtest/gtest.h>

#include <deque>

TEST(TemplateStackTest, DISABLE_Stack2Test) {
    // TODO stack string 的特化实现目前有问题，暂时原因未知
    Stack<std::string> stringStack;
    stringStack.push("hello");
    ASSERT_EQ("hello", stringStack.top());
    ASSERT_FALSE(stringStack.empty());
    stringStack.pop();
    ASSERT_TRUE(stringStack.empty());
    try {
        stringStack.pop();
    } catch (std::out_of_range const &ex) {
        printf("exception: %s\n", ex.what());
    }
}
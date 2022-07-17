#include "template/basics/stack4.hpp"

#include <gtest/gtest.h>

#include <deque>

TEST(TemplateStackTest, Stack4Test) {
    Stack<int, 20> int20Stack;
    int20Stack.push(7);
    ASSERT_EQ(7, int20Stack.top());
    int20Stack.pop();
    ASSERT_TRUE(int20Stack.empty());

    try {
        for (int i = 0; i < 21; ++i) {
            int20Stack.push(i);
        }
        FAIL();
    } catch (std::out_of_range const& ex) {
        std::cerr << "exception: " << ex.what() << std::endl;
    }

    Stack<std::string, 40> stringStack;
    stringStack.push("hello");
    ASSERT_EQ("hello", stringStack.top());
    stringStack.pop();
    ASSERT_TRUE(stringStack.empty());

    try {
        for (int i = 0; i < 41; ++i) {
            char tmp[5];
            snprintf(tmp, 5, "%d", i);
            stringStack.push(tmp);
        }
        FAIL();
    } catch (std::out_of_range const& ex) {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
}
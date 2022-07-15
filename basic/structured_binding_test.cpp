#include <gtest/gtest.h>
#include <string>

// https://en.cppreference.com/w/cpp/language/structured_binding
TEST(BindingTest, StructTest) {
    struct BindTest {
        int a = 42;
        std::string b = "hello structured binding";
    };

    BindTest bt;
    auto[x, y] = bt;
    ASSERT_EQ(42, x);
    ASSERT_EQ("hello structured binding", y);
}

TEST(BindingTest, ArrayTest) {
    int a[3]{1, 3, 5};

    auto[x, y, z] = a;
    ASSERT_EQ(1, x);
    ASSERT_EQ(3, y);
    ASSERT_EQ(5, z);
}

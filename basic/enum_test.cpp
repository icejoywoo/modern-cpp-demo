
#include <gtest/gtest.h>
#include <string>

TEST(EnumTest, BasicTest) {
    enum School {
        principal,
        teacher,
        student
    };

    enum Company {
        chairman,
        manager,
        employee
    };

    School x = student;
    Company y = manager;
    EXPECT_TRUE(x >= y);
    EXPECT_TRUE(student >= manager);
    EXPECT_EQ(2, student);
}

TEST(EnumTest, EnumValueComparationTest) {
    enum E{
        e1 = 1,
        e2 = 2,
        e3 = 3
    };

    bool b = e1 < e3;
    std::cout << std::boolalpha << b << std::endl;
}

template<int a, int b>
struct add {
    enum {
        result = a + b
    };
};

TEST(EnumTest, EnumHackTest) {
    // 可以在编译期完成计算
    EXPECT_EQ(13, (add<5, 8>::result));
    // 由于是编译期决定的，所以只能使用常量
    const int a = 5;
    const int b = 6;
    EXPECT_EQ(11, (add<a, b>::result));
}
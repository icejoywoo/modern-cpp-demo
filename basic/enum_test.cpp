
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"
    EXPECT_TRUE(x >= y);
    EXPECT_TRUE(student >= manager);
#pragma GCC diagnostic push
    EXPECT_EQ(2, student);
}

TEST(EnumTest, EnumValueComparationTest) {
//    enum E { // 编译器内部实现，不同编译器的行为可能不一致
    enum E : unsigned long { // since c++ 11 可以指定 enum 底层类型
        e1 = 1,
        e2 = 2,
        e3 = 0xfffffff0
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

TEST(EnumTest, StrongEnumTest) {
    // 强枚举类型 enum class
    // POD 类型
    enum class School {
        principal,
        teacher,
        student
    };

    enum class Company {
        chairman,
        manager,
        employee
    };

    School x = School::student;
    Company y = Company::manager;
    // enum class 无法隐式转换为 int，无法直接比较
    // 可以使用 static_cast 来强制转换，但是不推荐
    EXPECT_TRUE(static_cast<int>(x) >= static_cast<int>(y));
    EXPECT_TRUE(static_cast<int>(School::student) >= static_cast<int>(Company::manager));
    EXPECT_EQ(2, static_cast<int>(School::student)); // static_cast

    enum class E : unsigned long {
        e1 = 1,
        e2 = 2,
        e3 = 3
    };

    bool b = E::e1 < E::e3;
    std::cout << std::boolalpha << b << std::endl;
}
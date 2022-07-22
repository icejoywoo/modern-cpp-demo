// 奇异递归模板模式(Curiously Recurring Template Pattern，CRTP)
// https://zhuanlan.zhihu.com/p/54945314
// https://zhuanlan.zhihu.com/p/408668787
#include "gtest/gtest.h"

#include <string>

template<typename T>
class Base {
public:
    std::string foo() { return static_cast<T *>(this)->internal_foo(); }
    std::string bar() { return static_cast<T *>(this)->internal_bar(); }
};

class Derived1 : public Base<Derived1> {
public:
    std::string internal_foo() { return "Derived1 foo"; }
    std::string internal_bar() { return "Derived1 bar"; }
};

class Derived2 : public Base<Derived2> {
public:
    std::string internal_foo() { return "Derived2 foo"; }
    std::string internal_bar() { return "Derived2 bar"; }
};

template <typename T>
std::string foo(Base<T> &obj) { return obj.foo(); }

template <typename T>
std::string bar(Base<T> &obj) { return obj.bar(); }

TEST(CrtpTest, BasicTest) {
    // return value is string, copy strings multiple times
    Derived1 d1;
    Derived2 d2;

    ASSERT_EQ("Derived1 foo", foo(d1));
    ASSERT_EQ("Derived2 foo", foo(d2));
    ASSERT_EQ("Derived1 bar", bar(d1));
    ASSERT_EQ("Derived2 bar", bar(d2));
}
#include "gtest/gtest.h"

TEST(BindTest, BasicTest) {
    int numbers[] = { 10, 20, 30, 40, 50, 10 };
    int size = sizeof(numbers) / sizeof(int);
    // bind1st, bind2nd 已经废弃了，现在使用 bind
    // x < 40
    ASSERT_EQ(4, std::count_if(numbers, numbers + size, std::bind(std::less<int>(), std::placeholders::_1, 40)));
    // lambda 形式
    ASSERT_EQ(4, std::count_if(numbers, numbers + size, [](auto && PH1) { return std::less<int>()(std::forward<decltype(PH1)>(PH1), 40); }));
    // 40 < x
    ASSERT_EQ(1, std::count_if(numbers, numbers + size, std::bind(std::less<int>(), 40, std::placeholders::_1)));
    // lambda 形式
    ASSERT_EQ(1, std::count_if(numbers, numbers + size, [](auto && PH1) { return std::less<int>()(40, std::forward<decltype(PH1)>(PH1)); }));
}
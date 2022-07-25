#include "gtest/gtest.h"

#include <algorithm>
#include <vector>

TEST(BindTest, BasicTest) {
    int numbers[] = { 10, 20, 30, 40, 50, 10 };
    int size = sizeof(numbers) / sizeof(int);
    // bind1st, bind2nd 已经废弃了，现在使用 bind，c++ 17 删除了 bind1st, bind2nd
    // x < 40
    ASSERT_EQ(4, std::count_if(numbers, numbers + size, std::bind(std::less<int>(), std::placeholders::_1, 40)));
    // lambda 形式
    ASSERT_EQ(4, std::count_if(numbers, numbers + size, [](auto && PH1) { return std::less<int>()(std::forward<decltype(PH1)>(PH1), 40); }));
    // 40 < x
    ASSERT_EQ(1, std::count_if(numbers, numbers + size, std::bind(std::less<int>(), 40, std::placeholders::_1)));
    // lambda 形式
    ASSERT_EQ(1, std::count_if(numbers, numbers + size, [](auto && PH1) { return std::less<int>()(40, std::forward<decltype(PH1)>(PH1)); }));
}

TEST(BindTest, MemFunTest) {
    class Person {
    public:
        void Print() {
            std::cout << "Person::Print\n";
        }
    };

    // stl container 存放指针
    std::vector<std::shared_ptr<Person>> persons{std::make_shared<Person>()};
    // c++17 remove mem_fun & mem_fun_ref
    std::for_each(persons.begin(), persons.end(), std::mem_fn(&Person::Print));
}
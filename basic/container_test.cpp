#include <gtest/gtest.h>
#include <iostream>
#include <string>

class ContainerTest : public testing::Test
{
public:
    ContainerTest() {};
    virtual ~ContainerTest() {};

protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ContainerTest, VectorTest) {
    std::vector<std::string> list{"hello", "c++", "world"};
    for (auto s : list) {
        std::cout << s << std::endl;
    }
    std::cout << sizeof(std::string) << std::endl;

    std::vector<int> x1(5, 5); // {5, 5, 5, 5, 5}
    std::vector<int> x2{5, 5}; // {5, 5}

    // {} 外围的是 initializer_list，内部的 {"bear",4} 对应的是隐式构造函数调用
    std::map<std:: string, int> x8{ {"bear",4}, {"cassowary",2}, {"tiger",7} };
}
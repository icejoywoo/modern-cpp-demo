#include <gtest/gtest.h>
#include <iostream>
#include <string>

class InitializerListTest : public testing::Test
{
public:
    InitializerListTest() {};
    virtual ~InitializerListTest() {};

protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(InitializerListTest, CustomInitializerListTest) {
    struct C {
        C(std::initializer_list<std::string> a)
        {
            for (const std::string* item = a.begin(); item != a.end(); ++item) {
//                std::cout << *item << " ";
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    };

    C c{"hello", "c++", "world"};
    std::cout << sizeof(std::string) << std::endl;
}

TEST_F(InitializerListTest, NamedInitializerListTest) {
    {
        struct Point {
            int x;
            int y;
        };
        Point p{ .x = 4, .y = 2};
        ASSERT_EQ(4, p.x);
        ASSERT_EQ(2, p.y);
    }

    {
        struct Point {
            int x;
            int y;
            int z;
        };
        Point p{ .z = 3 };
        ASSERT_EQ(3, p.z);
        ASSERT_EQ(0, p.x);
        ASSERT_EQ(0, p.y);
    }
}
#include "template/basics/max.hpp"
#include "basic/common_util.h"

#include <gtest/gtest.h>

TEST(BasicsTest, MaxTest) {
    int i = 42;
    SIMPLE_PRINT(::max(7, i), "%d");
    double f1 = 3.4;
    double f2 = -6.7;
    SIMPLE_PRINT(::max(f1, f2), "%f");

    SIMPLE_PRINT(::max2(f1, i), "%f");

    SIMPLE_PRINT((::max3<double, int, double>(f1, i)), "%f");
    SIMPLE_PRINT((::max3<double>(f1, i)), "%f");

    std::string s1 = "mathematics";
    std::string s2 = "math";
    SIMPLE_PRINT(::max(s1, s2).c_str(), "%s");
}
//
// Created by icejoywoo on 2022/7/16.
//

#ifndef MODERN_CPP_DEMO_COMMON_UTIL_H
#define MODERN_CPP_DEMO_COMMON_UTIL_H

#define ASSERT_ARRAY_SIZE_AND_VALUE(array, size, initial_value) \
    ASSERT_EQ(size, sizeof array); \
    for (const auto& e : array) { \
        ASSERT_EQ(initial_value, e); \
    }

#define SIMPLE_PRINT(expr, format) \
    printf(#expr ": " #format "\n", expr)
#endif //MODERN_CPP_DEMO_COMMON_UTIL_H

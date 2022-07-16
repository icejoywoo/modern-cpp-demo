//
// Created by icejoywoo on 2022/7/16.
//

#ifndef MODERN_CPP_DEMO_COMMON_UTIL_H
#define MODERN_CPP_DEMO_COMMON_UTIL_H

#define STR_CONCAT(x, y) x##y

#define UNIQUE_NAME_IMPL(prefix, counter) STR_CONCAT(prefix, counter)

#define UNIQUE_NAME(prefix) UNIQUE_NAME_IMPL(prefix, __COUNTER__)

#define ASSERT_ARRAY_SIZE_AND_VALUE(array, size, initial_value) \
    ASSERT_EQ(size, sizeof array); \
    for (const auto& e : array) { \
        ASSERT_EQ(initial_value, e); \
    }

#define SIMPLE_PRINT_IMPL(var_name, expr, format) do { \
    auto var_name = (expr);                    \
    printf(#expr ": " format "\n", var_name); \
} while(0)

#define SIMPLE_PRINT(expr, format) SIMPLE_PRINT_IMPL(UNIQUE_NAME(simple_print_result_), expr, format)

#endif //MODERN_CPP_DEMO_COMMON_UTIL_H

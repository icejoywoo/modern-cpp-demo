#include "gtest/gtest.h"

#include <optional>

TEST(OptionalTest, BasicTest) {
    auto x = std::make_optional(3.0);
    ASSERT_TRUE(x.has_value());
    ASSERT_TRUE(x);
    ASSERT_EQ(3.0, x.value());
    std::optional<double> y = std::nullopt;
    ASSERT_FALSE(y.has_value());
    ASSERT_FALSE(y);
    ASSERT_EQ(0.0, y.value_or(0.0));
}

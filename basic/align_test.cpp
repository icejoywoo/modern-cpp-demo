#include "gtest/gtest.h"

// https://en.cppreference.com/w/cpp/language/alignas
TEST(AlignTest, BasicTest) {
  alignas(32) int a = 10;
  printf("a: %ld\n", (long) &a);
  ASSERT_EQ(0, (long) (&a) % 32);

  int b = 10;
  printf("b: %ld\n", (long) &b);
  EXPECT_NE(0, (long) (&b) % 32);
}

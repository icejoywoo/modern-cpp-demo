#include "LowerUpperImpl.h"

#include "gtest/gtest.h"

TEST(LowerUpperImplTest, UpperTest) {
  {
    const char* input = "ABCDEfghIJK";
    char dst[strlen(input) + 1];
    DB::LowerUpperImpl<'A', 'Z'>::array(input, input + strlen(input), dst);
    dst[strlen(input)] = '\0';
    ASSERT_STREQ("abcdefghijk", dst);
  }
  {
    const char* input = "ABCDEfghIJK";
    char dst[strlen(input) + 1];
    DB::LowerUpperImpl<'A', 'Z'>::simple_array(input, input + strlen(input), dst);
    dst[strlen(input)] = '\0';
    ASSERT_STREQ("abcdefghijk", dst);
  }
}

TEST(LowerUpperImplTest, LowerTest) {
  {
    const char* input = "ABCDEfghIJK";
    char dst[strlen(input) + 1];
    DB::LowerUpperImpl<'a', 'z'>::array(input, input + strlen(input), dst);
    dst[strlen(input)] = '\0';
    ASSERT_STREQ("ABCDEFGHIJK", dst);
  }
  {
    const char* input = "ABCDEfghIJK";
    char dst[strlen(input) + 1];
    DB::LowerUpperImpl<'a', 'z'>::simple_array(input, input + strlen(input), dst);
    dst[strlen(input)] = '\0';
    ASSERT_STREQ("ABCDEFGHIJK", dst);
  }
}
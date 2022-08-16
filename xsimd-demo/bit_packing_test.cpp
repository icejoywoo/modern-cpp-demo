#include "bit_packing.h"

#include "gtest/gtest.h"

namespace bit::packing {

TEST(BitPackingTest, BasicTest) {
  {
    uint8_t input[] = {0, 1, 0, 1};
    uint8_t expected[] = {0b01010000};
    uint8_t output[1] = {0};
    Simple::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    Multiplication::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    PackedStruct::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    BitHacks::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
#ifdef __SSE2__
    Sse2::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
#endif
  }

  {
    uint8_t input[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0};
    uint8_t expected[] = {0b01011010, 0b01011010};
    uint8_t output[2] = {0};
    Simple::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    Multiplication::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    PackedStruct::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    BitHacks::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
#ifdef __SSE2__
    Sse2::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
#endif
  }

  {
    uint8_t input[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1};
    uint8_t expected[] = {0b01011010, 0b01000000};
    uint8_t output[2] = {0};
    Simple::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    Multiplication::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    PackedStruct::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
    BitHacks::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
#ifdef __SSE2__
    Sse2::encodeAsBits(input, sizeof(input) / sizeof(char), output);
    EXPECT_EQ(expected[0], output[0]);
    EXPECT_EQ(expected[1], output[1]);
#endif
  }
}

} // namespace bit::packing

#pragma once
#include <cstdint>

#include <emmintrin.h>

namespace bit::packing {

class Simple {
 public:
  static void encodeAsBits(uint8_t* input, int input_length,
                           uint8_t* output) {
    int idx = 0;

    int batch_size = input_length & ~0b111;
    int left_size = input_length & 0b111;

    for (int position = 0; position < batch_size; position += 8) {
      unsigned value = 0;
      value |= input[position] == 1 ? 0b10000000 : 0;
      value |= input[position + 1] == 1 ? 0b01000000 : 0;
      value |= input[position + 2] == 1 ? 0b00100000 : 0;
      value |= input[position + 3] == 1 ? 0b00010000 : 0;
      value |= input[position + 4] == 1 ? 0b00001000 : 0;
      value |= input[position + 5] == 1 ? 0b00000100 : 0;
      value |= input[position + 6] == 1 ? 0b00000010 : 0;
      value |= input[position + 7] == 1 ? 0b00000001 : 0;
      output[idx++] = value;
    }

    // write last null bits
    if (left_size > 0) {
      unsigned char value = 0;
      int mask = 0b10000000;
      for (int position = batch_size; position < input_length; position++) {
        value |= input[position] == 1 ? mask : 0;
        mask >>= 1;
      }
      output[idx] = value;
    }
  }
};

// The cool way (using the multiplication technique)
// https://stackoverflow.com/questions/8461126/how-to-create-a-byte-out-of-8-bool-values-and-vice-versa
// https://stackoverflow.com/questions/14537831/isolate-specific-row-column-diagonal-from-a-64-bit-number/14539116#14539116

const uint64_t column_mask = 0x8080808080808080ull;
const uint64_t magic = 0x2040810204081ull;

int get_col(uint64_t board, int col) {
  uint64_t column = (board << col) & column_mask;
  column *= magic;
  return (column >> 56) & 0xff;
}

int get_col(uint64_t board) {
  uint64_t column = (board << 7) & column_mask;
  column *= magic;
  return (column >> 56) & 0xff;
}

class Multiplication {
 public:
  static void encodeAsBits(uint8_t* input, int input_length,
                           uint8_t* output) {
    int idx = 0;

    int batch_size = input_length & ~0b111;
    int left_size = input_length & 0b111;

    for (int position = 0; position < batch_size; position += 8) {
      output[idx++] = get_col(*reinterpret_cast<uint64_t*>(&input[position]));
    }

    // write last null bits
    if (left_size > 0) {
      unsigned char value = 0;
      int mask = 0b10000000;
      for (int position = batch_size; position < input_length; position++) {
        value |= input[position] == 1 ? mask : 0;
        mask >>= 1;
      }
      output[idx] = value;
    }
  }
};

struct bool8 {
  uint8_t val0 : 1;
  uint8_t val1 : 1;
  uint8_t val2 : 1;
  uint8_t val3 : 1;
  uint8_t val4 : 1;
  uint8_t val5 : 1;
  uint8_t val6 : 1;
  uint8_t val7 : 1;
};

class PackedStruct {
 public:
  static void encodeAsBits(uint8_t* input, int input_length,
                           uint8_t* output) {
    int idx = 0;

    int batch_size = input_length & ~0b111;
    int left_size = input_length & 0b111;

    for (int position = 0; position < batch_size; position += 8) {
      bool8 value;
      value.val0 = input[position] == 1 ? 1 : 0;
      value.val1 = input[position + 1] == 1 ? 1 : 0;
      value.val2 = input[position + 2] == 1 ? 1 : 0;
      value.val3 = input[position + 3] == 1 ? 1 : 0;
      value.val4 = input[position + 4] == 1 ? 1 : 0;
      value.val5 = input[position + 5] == 1 ? 1 : 0;
      value.val6 = input[position + 6] == 1 ? 1 : 0;
      value.val7 = input[position + 7] == 1 ? 1 : 0;
      output[idx++] = *reinterpret_cast<uint8_t*>(&value);
    }

    // write last null bits
    if (left_size > 0) {
      unsigned char value = 0;
      int mask = 0b10000000;
      for (int position = batch_size; position < input_length; position++) {
        value |= input[position] == 1 ? mask : 0;
        mask >>= 1;
      }
      output[idx] = value;
    }
  }
};

#ifdef __SSE2__
// https://stackoverflow.com/questions/8461126/how-to-create-a-byte-out-of-8-bool-values-and-vice-versa
inline uint8_t pack8bools_SSE2(const uint8_t* a)
{
  __m128i v = _mm_loadl_epi64( (const __m128i*)a );  // 8-byte load, despite the pointer type.
  // __m128 v = _mm_cvtsi64_si128( uint64 );  // alternative if you already have an 8-byte integer
  v = _mm_slli_epi32(v, 7);   // low bit of each byte becomes the highest
  return _mm_movemask_epi8(v);
}
#endif

#ifdef __SSE2__
class Sse2 {
 public:
  static void encodeAsBits(uint8_t* input, int input_length,
                           uint8_t* output) {
    int idx = 0;

    int batch_size = 0;
    int left_size = 0;

    batch_size= (input_length - batch_size) & ~0b111;
    left_size = (input_length - batch_size) & 0b111;

    for (int position = 0; position < batch_size; position += 8) {
      output[idx++] = pack8bools_SSE2(&input[position]);
    }

    // write last null bits
    if (left_size > 0) {
      unsigned char value = 0;
      int mask = 0b10000000;
      for (int position = batch_size; position < input_length; position++) {
        value |= input[position] == 1 ? mask : 0;
        mask >>= 1;
      }
      output[idx] = value;
    }
  }
};
#endif

} // namespace bit::packing
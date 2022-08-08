#include "gtest/gtest.h"

#include <atomic>
#include <cstdint>

// copied from velox/vector/Buffer.h

struct BufferStruct {
  uint8_t* pool_;
  uint8_t* data_;
  uint64_t size_ = 0;
  uint64_t capacity_ = 0;
  std::atomic<int32_t> referenceCount_;
  bool mutable_ = true;
  bool podType_ = true;
  // Pad to 64 bytes. If using as int32_t[], guarantee that value at index -1 ==
  // -1.
  uint64_t padding_[2] = {static_cast<uint64_t>(-1), static_cast<uint64_t>(-1)};
};

class BufferWithVirtual {
public:
  uint8_t* pool_;
  uint8_t* data_;
  uint64_t size_ = 0;
  uint64_t capacity_ = 0;
  std::atomic<int32_t> referenceCount_;
  bool mutable_ = true;
  bool podType_ = true;
  // Pad to 64 bytes. If using as int32_t[], guarantee that value at index -1 ==
  // -1.
  uint64_t padding_[2] = {static_cast<uint64_t>(-1), static_cast<uint64_t>(-1)};

  virtual ~BufferWithVirtual(){};
};

static_assert(
    sizeof(BufferWithVirtual) == 64,
    "Buffer is assumed to be 64 bytes to guarantee alignment");

TEST(SizeofTest, BufferTest) {
    std::atomic<int32_t> atomic_int32;
    ASSERT_EQ(4, sizeof(atomic_int32));
    BufferStruct buffer_struct;
    ASSERT_EQ(56, sizeof(buffer_struct));
    BufferWithVirtual buffer;
    ASSERT_EQ(64, sizeof(buffer));
}

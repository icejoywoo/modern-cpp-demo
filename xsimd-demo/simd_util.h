#ifndef MODERN_CPP_DEMO_SIMD_UTIL_H
#define MODERN_CPP_DEMO_SIMD_UTIL_H

#include "xsimd/xsimd.hpp"

#include <cstdint>

// copied from velox/common/base/SimdUtil.h
// changes: change bytes type from int32_t to size_t to remove compile warnings
namespace simd {

// Use for initialization with memset.
constexpr char kNullByte = 0;
constexpr char kNotNullByte = 0xff;

// Return width of the widest store.
template <typename A = xsimd::default_arch>
constexpr int32_t batchByteSize(const A& = {}) {
  return sizeof(xsimd::types::simd_register<int8_t, A>);
}

namespace detail {

// Adds 'bytes' bytes to an address of arbitrary type.
template <typename T>
inline T* addBytes(T* pointer, size_t bytes) {
  return reinterpret_cast<T*>(reinterpret_cast<uint64_t>(pointer) + bytes);
}

template <typename T, typename A>
struct SetWord {
  static void apply(void* to, T data) { *reinterpret_cast<T*>(to) = data; }
};

template <typename A>
struct SetWord<xsimd::batch<int8_t, A>, A> {
  static void apply(void* to, xsimd::batch<int8_t, A> data) {
    data.store_unaligned(reinterpret_cast<int8_t*>(to));
  }
};

template <typename T, typename A>
inline bool setNextWord(void*& to, T data, size_t& bytes, const A&) {
  if (bytes >= sizeof(T)) {
    SetWord<T, A>::apply(to, data);
    bytes -= sizeof(T);
    if (!bytes) {
      return false;
    }
    to = addBytes(to, sizeof(T));
    return true;
  }
  return true;
}

}  // namespace detail

// memset implementation that writes at maximum width and unrolls for
// constant values of 'bytes'.
template <typename A = xsimd::default_arch>
void memset(void* to, char data, size_t bytes, const A& arch = {}) {
  auto v = xsimd::batch<int8_t, A>::broadcast(data);
  while (bytes >= batchByteSize(arch)) {
    if (!detail::setNextWord(to, v, bytes, arch)) {
      return;
    }
  }
  int64_t data64 = *reinterpret_cast<int64_t*>(&v);
  while (bytes >= sizeof(int64_t)) {
    if (!detail::setNextWord<int64_t>(to, data64, bytes, arch)) {
      return;
    }
  }
  if (!detail::setNextWord<int32_t>(to, data64, bytes, arch)) {
    return;
  }
  if (!detail::setNextWord<int16_t>(to, data64, bytes, arch)) {
    return;
  }
  detail::setNextWord<int8_t>(to, data64, bytes, arch);
}

namespace detail {

template <typename T, typename A>
struct CopyWord {
  static void apply(void* to, const void* from) {
    *reinterpret_cast<T*>(to) = *reinterpret_cast<const T*>(from);
  }
};

template <typename A>
struct CopyWord<xsimd::batch<int8_t, A>, A> {
  static void apply(void* to, const void* from) {
    xsimd::batch<int8_t, A>::load_unaligned(
        reinterpret_cast<const int8_t*>(from))
        .store_unaligned(reinterpret_cast<int8_t*>(to));
  }
};

// Copies one element of T and advances 'to', 'from', and 'bytes' by
// sizeof(T). Returns false if 'bytes' went to 0.
template <typename T, typename A>
inline bool copyNextWord(void*& to, const void*& from, size_t& bytes) {
  if (bytes >= sizeof(T)) {
    CopyWord<T, A>::apply(to, from);
    bytes -= sizeof(T);
    if (!bytes) {
      return false;
    }
    from = addBytes(from, sizeof(T));
    to = addBytes(to, sizeof(T));
    return true;
  }
  return true;
}
} // namespace detail

// 'memcpy' implementation that copies at maximum width and unrolls
// when 'bytes' is constant.
template <typename A = xsimd::default_arch>
void memcpy(void* to, const void* from, size_t bytes, const A& arch = {}) {
  while (bytes >= batchByteSize(arch)) {
    if (!detail::copyNextWord<xsimd::batch<int8_t, A>, A>(to, from, bytes)) {
      return;
    }
  }
  while (bytes >= sizeof(int64_t)) {
    if (!detail::copyNextWord<int64_t, A>(to, from, bytes)) {
      return;
    }
  }
  if (!detail::copyNextWord<int32_t, A>(to, from, bytes)) {
    return;
  }
  if (!detail::copyNextWord<int16_t, A>(to, from, bytes)) {
    return;
  }
  detail::copyNextWord<int8_t, A>(to, from, bytes);
}

} // namespace simd

#endif  // MODERN_CPP_DEMO_SIMD_UTIL_H

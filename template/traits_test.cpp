#include "gtest/gtest.h"

template <typename T>
inline T sigma1(T const* start, T const* end) {
    T result = T();
    while (start != end) {
        result += *start++;
    }
    return result;
}

TEST(TraitsTest, WrongTest) {
    int array[] = {1, 2, 3, 4, 5};
    size_t array_size = sizeof(array) / sizeof(int);
    ASSERT_EQ(15, sigma1(array, array + array_size));

    // 结果溢出：97 + 98 + 99 = 294
    // 294 - 256 = 38, 对应为 ascii '&'
    const char* char_array = "abc";
    ASSERT_EQ('&', sigma1(char_array, char_array + strlen(char_array)));
}

template <typename T> class SigmaTraits {
public:
    typedef T ReturnType;
};

template <>
class SigmaTraits<char> {
public:
    typedef short ReturnType;
};

template <>
class SigmaTraits<int> {
public:
    typedef long ReturnType;
};

template <>
class SigmaTraits<long> {
public:
    typedef long long ReturnType;
};

template <>
class SigmaTraits<float> {
public:
    typedef double ReturnType;
};

// 通过 SigmaTraits 的特化来完成 ReturnType 的推导，将其类型向上升级保证不溢出
template <typename T>
inline typename SigmaTraits<T>::ReturnType sigma(T const* start, T const* end) {
    typedef typename SigmaTraits<T>::ReturnType ReturnType;
    ReturnType result = ReturnType();
    while (start != end) {
        result += *start++;
    }
    return result;
}

TEST(TraitsTest, BasicTest) {
    int array[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(15, sigma(array, array + 5));

    const char* char_array = "abc";
    ASSERT_EQ(294, sigma(char_array, char_array + 3));

}
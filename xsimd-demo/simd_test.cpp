#include <gtest/gtest.h>

#include "xsimd/xsimd.hpp"

#include <vector>

namespace xs = xsimd;

using vector_type = std::vector<double, xsimd::aligned_allocator<double>>;

void mean(const vector_type& a, const vector_type& b, vector_type& res)
{
    std::size_t size = a.size();
    constexpr std::size_t simd_size = xsimd::simd_type<double>::size;
    std::size_t vec_size = size - size % simd_size;

    for(std::size_t i = 0; i < vec_size; i += simd_size)
    {
        auto ba = xs::load_aligned(&a[i]);
        auto bb = xs::load_aligned(&b[i]);
        auto bres = (ba + bb) / 2;
        bres.store_aligned(&res[i]);
    }
    for(std::size_t i = vec_size; i < size; ++i)
    {
        res[i] = (a[i] + b[i]) / 2;
    }
}

TEST(XsimdTest, BasicTest) {
#ifdef __AVX__
    {
        xs::batch<double, xs::avx> a = {1.5, 2.5, 3.5, 4.5};
        xs::batch<double, xs::avx> b = {2.5, 3.5, 4.5, 5.5};
        auto mean = (a + b) / 2;
        std::stringstream ss;
        ss << mean;
        ASSERT_EQ("(2, 3, 4, 5)", ss.str());
    }
#endif

    {
        vector_type res(4);
        mean({1.5, 2.5, 3.5, 4.5}, {2.5, 3.5, 4.5, 5.5}, res);
        std::stringstream ss;
        ASSERT_EQ(4, res.size());
        std::copy(res.begin(), res.end(), std::ostream_iterator<int>(ss, ", "));
        ASSERT_EQ("2, 3, 4, 5, ", ss.str());
        ASSERT_EQ("\2\3\4\5", std::string(res.begin(), res.end()));
    }
}

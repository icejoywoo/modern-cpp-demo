#include <gtest/gtest.h>

#include "xsimd/xsimd.hpp"
#include "simdjson.h"

#include <vector>

namespace xs = xsimd;
using namespace simdjson;

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
    {
        xs::batch<double, xs::avx> a = {1.5, 2.5, 3.5, 4.5};
        xs::batch<double, xs::avx> b = {2.5, 3.5, 4.5, 5.5};
        auto mean = (a + b) / 2;
        std::cout << mean << std::endl;
    }

    {
        // 不报错，但是结果没写到res里面
        vector_type res;
        res.reserve(4);
        mean({1.5, 2.5, 3.5, 4.5}, {2.5, 3.5, 4.5, 5.5}, res);
        std::cout << std::string(res.begin(), res.end()) << std::endl;
    }
}

TEST(SimdjsonTest, BasicTest) {
    ondemand::parser parser;
    {
        auto json = "[1,2,3]"_padded; // The _padded suffix creates a simdjson::padded_string instance
        ondemand::document doc = parser.iterate(json); // parse a string
        ASSERT_EQ(1, int64_t(doc.at(0)));
        ASSERT_EQ(2, int64_t(doc.at(1)));
        ASSERT_EQ(3, int64_t(doc.at(2)));
    }

    {
        // 需要预留一个 SIMDJSON_PADDING 大小的额外空间
        char json[3 + SIMDJSON_PADDING];
        strcpy(json, "[1]");
        ondemand::document doc = parser.iterate(json, strlen(json), sizeof(json));
        ASSERT_EQ(1, int64_t(doc.at(0)));
    }

    {
        // 需要预留一个 SIMDJSON_PADDING 大小的额外空间
        std::string json("[1]");
        auto size = json.size();
        json.reserve(size + SIMDJSON_PADDING);
        ASSERT_TRUE(json.capacity() - json.size() >= SIMDJSON_PADDING);
        ondemand::document doc = parser.iterate(json);
        ASSERT_EQ(1, int64_t(doc.at(0)));
    }
}

TEST(SimdjsonTest, ToStringStringTest) {
    auto cars_json = R"( [
  { "make": "Toyota", "model": "Camry",  "year": 2018, "tire_pressure": [ 40.1, 39.9, 37.7, 40.4 ] },
  { "make": "Kia",    "model": "Soul",   "year": 2012, "tire_pressure": [ 30.1, 31.0, 28.6, 28.7 ] },
  { "make": "Toyota", "model": "Tercel", "year": 1999, "tire_pressure": [ 29.8, 30.0, 30.2, 30.5 ] }
] )"_padded;
    std::vector<std::string_view> arrays;
// We are going to collect string_view instances which point inside the `cars_json` string
// and are therefore valid as long as `cars_json` remains in scope.
    {
        ondemand::parser parser;
        for (ondemand::object car : parser.iterate(cars_json)) {
            if(uint64_t(car["year"]) > 2000) {
                arrays.push_back(simdjson::to_json_string(car["tire_pressure"]));
            }
        }
    }
// We can now convert to a JSON string:
    std::ostringstream oss;
    oss << "[";
    for(size_t i = 0; i < arrays.size(); i++) {
        if(i>0) { oss << ","; }
        oss << arrays[i];
    }
    oss << "]";
    auto json_string = oss.str();
// json_string == "[[ 40.1, 39.9, 37.7, 40.4 ],[ 30.1, 31.0, 28.6, 28.7 ]]"
    ASSERT_EQ("[[ 40.1, 39.9, 37.7, 40.4 ],[ 30.1, 31.0, 28.6, 28.7 ]]", json_string);
}
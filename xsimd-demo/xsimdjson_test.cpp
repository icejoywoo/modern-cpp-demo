#include <gtest/gtest.h>

#include "simdjson.h"

#include <vector>

using namespace simdjson;

TEST(SimdjsonTest, BasicTest) {
  ondemand::parser parser;
  {
    auto json = "[1,2,3]"_padded; // The _padded suffix creates a simdjson::padded_string instance
    ondemand::document doc = parser.iterate(json); // parse a string
    // array 的 at 方法只能调用一次
    // int64_t(array.at(0)) == 1
    auto array = doc.get_array();
    int expected = 1;
    for (auto elem : array) {
      ASSERT_EQ(expected++, int64_t(elem));
    }
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
#include "gtest/gtest.h"

#include "folly/dynamic.h"
#include "folly/json.h"

/**
 * folly::dynamic supported types:
 * enum Type {
 *   NULLT,    // nullptr_t
 *   ARRAY,    // std::vector<folly::dynamic>
 *   BOOL,     // bool
 *   DOUBLE,   // double
 *   INT64,    // int64_t
 *   OBJECT,   // 类似于 std::unordered_map<folly::dynamic, folly::dynamic>
 *   STRING,   // std::string
 * };
 *
 * https://zhiqiang.org/coding/folly-dynamic.html
 */
TEST(FollyTest, DynamicTest) {
    folly::dynamic a = 7;
    folly::dynamic n = nullptr;
    folly::dynamic b = true;
    folly::dynamic f = 1.2;
    folly::dynamic i = 1;
    folly::dynamic s = "abc";
    folly::dynamic arr = folly::dynamic::array(n, i, s, true);
    folly::dynamic o = folly::dynamic::object(b, f)(f, arr);

    ASSERT_TRUE(a.isInt());
    ASSERT_TRUE(n.isNull());
    ASSERT_TRUE(b.isBool());
    ASSERT_TRUE(f.isDouble());
    ASSERT_TRUE(i.isInt());
    ASSERT_TRUE(s.isString());
    ASSERT_TRUE(arr.isArray());
    ASSERT_TRUE(o.isObject());

    ASSERT_EQ(folly::dynamic::Type::INT64, a.type());

    ASSERT_EQ(7, a.getInt());
    ASSERT_EQ(1, arr[1].asInt());

    // asString
    ASSERT_EQ("7", a.asString());

    // {1.2:[null,1,"abc",true],true:1.2}
    std::cout << o << std::endl;

    // iterate array
    std::cout << "array: \n";
    for (auto item : arr) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // folly json opt
    folly::json::serialization_opts json_opts;
    json_opts.allow_non_string_keys = true;
    ASSERT_EQ("{1.2:[null,1,\"abc\",true],true:1.2}", folly::json::serialize(o, json_opts));

    // iterate object
    // o.keys, o.values 与 python 很类似
    std::cout << "object: \n";
    for (auto [key, value] : o.items()) {
        std::cout << key << ": " << value << ", ";
    }
    std::cout << std::endl;
}

TEST(FollyTest, JsonTest) {
    // folly json key must be string
    const char* test_json = R"({"a": true, "b": 1, "c": [1, 2, 3]})";
    auto object = folly::parseJson(test_json);
    ASSERT_TRUE(object.isObject());
    ASSERT_TRUE(object["a"].asBool());
    ASSERT_TRUE(object["a"].asBool());
    ASSERT_TRUE(object["a"].asBool());
}

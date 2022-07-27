#include "gtest/gtest.h"

#include "folly/Conv.h"
#include "folly/container/F14Map.h"

#include <string>

TEST(FollyTest, F14FastMapTest) {
    folly::F14FastMap<std::string, std::string> map;
    map.reserve(10);
    std::vector<std::pair<std::string const, std::string>> v(
            {{"abc", "first"}, {"abc", "second"}});
    map.insert(v.begin(), v.end());
    EXPECT_EQ(1, map.size());
    // 这里是第一次插入的值生效
    EXPECT_EQ("first", map["abc"]);
    map["foo"] = "bar";
    map[folly::to<std::string>(1)] = folly::to<std::string>(1);
    EXPECT_EQ(3, map.size());

    EXPECT_EQ("1", map["1"]);
    EXPECT_FALSE(map.contains("not_found"));
}
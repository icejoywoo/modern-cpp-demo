#include "gtest/gtest.h"

#include "common/safe_bool_base.hpp"

class TruthTestableWithVirtualFunction : public SafeBool<> {
 protected:
  bool BooleanTest() const /* override */ {
    // Perform boolean logic here
    return true;
  }
};

class TruthTestableCRTP : public SafeBool<TruthTestableCRTP> {
 public: // 注意这里一定要是public
  /* NOT virtual */ bool BooleanTest() const {
    // Perform boolean logic here
    return true;
  }
};

TEST(SafeBoolTest, BasicTest) {
    TruthTestableWithVirtualFunction a;
    ASSERT_TRUE(a);
    TruthTestableCRTP b;
    ASSERT_TRUE(b);
}

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

// simplest way since c++ 11
// explicit operator bool
class TruthTestable {
 public:
  explicit TruthTestable(bool ok) : ok_{ok} {}
  explicit operator bool() const { return ok_; }

 private:
  bool ok_;
};

TEST(SafeBoolTest, BasicTest) {
    TruthTestableWithVirtualFunction a;
    ASSERT_TRUE(a);
    TruthTestableCRTP b;
    ASSERT_TRUE(b);
    TruthTestable c(false);
    ASSERT_FALSE(c);
}

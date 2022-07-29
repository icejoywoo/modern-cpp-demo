#include "gtest/gtest.h"

#include <memory>


class Singleton {
 public:
  // arrow & velox singleton implementation
  static std::shared_ptr<Singleton> GetInstance() {
    static std::shared_ptr<Singleton> instance(new Singleton);
    return instance;
  }
 private:
  Singleton() = default;
};

TEST(SingletonTest, BasicTest) {
  // singleton test
  std::shared_ptr<Singleton> singleton_a = Singleton::GetInstance();
  std::shared_ptr<Singleton> singleton_b = Singleton::GetInstance();
  ASSERT_EQ(singleton_a.get(), singleton_b.get());
}
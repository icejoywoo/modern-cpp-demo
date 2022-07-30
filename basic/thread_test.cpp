#include "gtest/gtest.h"

#include <thread>

static std::atomic<int> sum{0};

void f(int n) {
  for (int i = 0; i < n; ++i) {
    sum++;
  }
}

class functor {
 public:
  void operator()(int n) {
    for (int i = 0; i < n; ++i) {
      sum++;
    }
  }
};

TEST(ThreadTest, BasicTest) {
  unsigned int n = std::thread::hardware_concurrency();
  ASSERT_GT(n, 0);

  std::thread t1(f, 10);
  int a = 20;
  std::thread t2(functor(), std::ref(a));
  t1.join();
  t2.join();

  ASSERT_EQ(30, sum);
}

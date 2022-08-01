#include "gtest/gtest.h"

#include <random>
#include <string>
#include <vector>

template <typename T>
void print_vector(std::vector<T> vec) {
  std::stringstream ss;
  for (int i = 0; i < vec.size(); ++i) {
    if (i != 0) {
      ss << ", ";
    }
    ss << vec[i];
  }
  printf("%s\n", ss.str().c_str());
}

TEST(AlgorithmTest, GenerateTest) {
  std::vector<std::string> vec(100);
  std::generate(vec.begin(), vec.end(), [] {
    static int i = 0;
    return std::to_string(i++);
  });

  ASSERT_EQ(100, vec.size());
  for (int i = 0; i < vec.size(); ++i) {
    ASSERT_EQ(std::to_string(i), vec[i]);
  }
}

// https://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
TEST(AlgorithmTest, RandomGenerateTest) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int> dist {1, 52};
  std::vector<int> vec(100);
  std::generate(vec.begin(), vec.end(), [&dist, &mersenne_engine] {
    return dist(mersenne_engine);
  });

  ASSERT_EQ(100, vec.size());
  print_vector(vec);

  // rearrange elements
  std::shuffle(vec.begin(), vec.end(), mersenne_engine);
  print_vector(vec);
}
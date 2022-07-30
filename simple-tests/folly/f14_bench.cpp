#include "folly/Benchmark.h"
#include "folly/Conv.h"
#include "folly/container/F14Map.h"
#include "folly/container/Foreach.h"

#include <map>
#include <string>

BENCHMARK(std_unordered_map, n) {
  std::unordered_map<std::string, std::string> map;
  FOR_EACH_RANGE (i, 0, n) {
    map[folly::to<std::string>(i)] = folly::to<std::string>(i);
  }

  FOR_EACH_RANGE (i, 0, n) {
    map.erase(folly::to<std::string>(i));
  }
  folly::doNotOptimizeAway(map);
}

BENCHMARK_RELATIVE(std_map, n) {
  std::map<std::string, std::string> map;
  FOR_EACH_RANGE (i, 0, n) {
    map[folly::to<std::string>(i)] = folly::to<std::string>(i);
  }

  FOR_EACH_RANGE (i, 0, n) {
    map.erase(folly::to<std::string>(i));
  }
  folly::doNotOptimizeAway(map);
}

BENCHMARK_RELATIVE(folly_F14FastMap, n) {
  folly::F14FastMap<std::string, std::string> map;
  FOR_EACH_RANGE (i, 0, n) {
    map[folly::to<std::string>(i)] = folly::to<std::string>(i);
  }

  FOR_EACH_RANGE (i, 0, n) {
    map.erase(folly::to<std::string>(i));
  }
  folly::doNotOptimizeAway(map);
}

/**
 * bench result:
============================================================================
../simple-tests/folly/f14_bench.cpp             relative  time/iter  iters/s
============================================================================
std_unordered_map                                          155.05ns    6.45M
std_map                                           63.85%   242.84ns    4.12M
folly_F14FastMap                                 137.99%   112.36ns    8.90M
 */
int main() {
  folly::runBenchmarks();
}
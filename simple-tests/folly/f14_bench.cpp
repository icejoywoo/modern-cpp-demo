#include "folly/Benchmark.h"
#include "folly/Conv.h"
#include "folly/container/F14Map.h"
#include "folly/container/Foreach.h"

#include <map>
#include <string>

BENCHMARK(std_map, n) {
  std::map<std::string, std::string> map;
  FOR_EACH_RANGE (i, 0, n) {
    map[folly::to<std::string>(i)] = folly::to<std::string>(i);
  }

  FOR_EACH_RANGE (i, 0, n) {
    map.erase(folly::to<std::string>(i));
  }
}

BENCHMARK_RELATIVE(folly_F14FastMap, n) {
  folly::F14FastMap<std::string, std::string> map;
  FOR_EACH_RANGE (i, 0, n) {
    map[folly::to<std::string>(i)] = folly::to<std::string>(i);
  }

  FOR_EACH_RANGE (i, 0, n) {
    map.erase(folly::to<std::string>(i));
  }
}

/**
 * bench result:
 * ============================================================================
 * simple-tests/folly/f14_bench.cpp                 relative  time/iter  iters/s
 * ============================================================================
 * std_map                                                    322.34ns    3.10M
 * folly_F14FastMap                                 236.35%   136.38ns    7.33M
 */
int main() {
  folly::runBenchmarks();
}
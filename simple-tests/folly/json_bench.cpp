#include "folly/Benchmark.h"
#include "folly/container/Foreach.h"
#include "folly/json.h"
#include "simdjson.h"
#include "nlohmann/json.hpp"

constexpr const char* test_json = R"({
    "created_at": "Sun Aug 31 00:29:15 +0000 2014",
    "id": 505874924095815681,
    "id_str": "505874924095815681",
    "text": "@aym0566x \n\n名前:前田あゆみ\n第一印象:なんか怖っ！\n今の印象:とりあえずキモい。噛み合わない\n好きなところ:ぶすでキモいとこ😋✨✨\n思い出:んーーー、ありすぎ😊❤️\nLINE交換できる？:あぁ……ごめん✋\nトプ画をみて:照れますがな😘✨\n一言:お前は一生もんのダチ💖"
})";

BENCHMARK(folly_json, n) {
  folly::StringPiece text;
  FOR_EACH_RANGE (i, 0, n) {
    folly::dynamic object = folly::parseJson(test_json);
    text = object["text"].stringPiece();
  }
  folly::doNotOptimizeAway(text);
}

BENCHMARK_RELATIVE(simd_json, n) {
  std::string_view text;
  simdjson::ondemand::parser parser;
  FOR_EACH_RANGE (i, 0, n) {
    // add SIMDJSON_PADDING
    std::string json(test_json);
    json.reserve(json.size() + simdjson::SIMDJSON_PADDING);
    simdjson::ondemand::document doc = parser.iterate(json); // parse a string
    text = doc["text"].get_string();
  }
  folly::doNotOptimizeAway(text);
}

BENCHMARK_RELATIVE(simd_json_without_prepare, n) {
  std::string_view text;
  // add SIMDJSON_PADDING
  std::string json(test_json);
  json.reserve(json.size() + simdjson::SIMDJSON_PADDING);
  simdjson::ondemand::parser parser;
  FOR_EACH_RANGE (i, 0, n) {
    simdjson::ondemand::document doc = parser.iterate(json); // parse a string
    text = doc["text"].get_string();
  }
  folly::doNotOptimizeAway(text);
}

BENCHMARK_RELATIVE(nlohmann_json, n) {
  std::string text;
  FOR_EACH_RANGE (i, 0, n) {
    nlohmann::json json = nlohmann::json::parse(test_json);
    text = json["text"];
  }
  folly::doNotOptimizeAway(text);
}

/**
 * bench result:
============================================================================
../simple-tests/folly/json_bench.cpp            relative  time/iter  iters/s
============================================================================
folly_json                                                  16.37us   61.07K
simd_json                                       6674.49%   245.32ns    4.08M
simd_json_without_prepare                       8846.40%   185.09ns    5.40M
nlohmann_json                                    458.47%     3.57us  280.00K
 */
int main() {
  folly::runBenchmarks();
}
//
// Created by icejoywoo on 2022/7/18.
//

#include "basics/tracer.hpp"

#include <gtest/gtest.h>

TEST(TracerTest, SortTest) {
    SortTracer input[] = { 7, 3, 5, 6, 4, 2, 0, 1, 9, 8 };

    // 输出初始值:
    for (int i=0; i<10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << std::endl;

    long created_at_start = SortTracer::creations();
    long destroyed_at_start = SortTracer::destructions();
    long assigned_at_start = SortTracer::assignments();
    long compared_at_start = SortTracer::comparisons();
    long max_live_at_start = SortTracer::max_live();

    // 执行算法:
    std::cerr << "---[ Start std::sort() ]--------------------\n";
    std::sort<>(&input[0], &input[9]+1);
    std::cerr << "---[ End std::sort() ]----------------------\n";

    // 确认结果:
    for (int i=0; i<10; ++i) {
        std::cerr << input[i].val() << ' ';
    }
    std::cerr << "\n\n";

    // 最后的输出报告:
    std::cerr << "std::sort() of 10 SortTracer's"
              << " was performed by:\n "
              << SortTracer::creations() - created_at_start
              << " temporary tracers\n "
              << "up to "
              << SortTracer::max_live()
              << " tracers at the same time ("
              << max_live_at_start << " before)\n "
              << SortTracer::assignments() - assigned_at_start
              << " assignments\n "
              << SortTracer::comparisons() - compared_at_start
              << " comparisons\n\n";
}
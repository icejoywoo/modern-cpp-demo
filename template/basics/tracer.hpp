//
// Created by icejoywoo on 2022/7/18.
//

#ifndef MODERN_CPP_DEMO_TRACKER_HPP
#define MODERN_CPP_DEMO_TRACKER_HPP

#include <iostream>

// example from c++ templates 6.6.4
// 一个 sort tracer，用来测试验证 sort 模板函数的辅助类
class SortTracer {
private:
    int value;
    int generation; // 拷贝数
    static long n_created; // 构造函数
    static long n_destroyed; // 析构函数
    static long n_assigned; // 赋值
    static long n_compared; // 比较
    static long n_max_live; // 存活对象的最大个数

    static void update_max_live() {
        if (n_created - n_destroyed > n_max_live) {
            n_max_live = n_created - n_destroyed;
        }
    }

public:
    static long creations() {
        return n_created;
    }

    static long destructions() {
        return n_destroyed;
    }

    static long assignments() {
        return n_assigned;
    }

    static long comparisons() {
        return n_compared;
    }

    static long max_live() {
        return n_max_live;
    }

public:
    SortTracer(int v = 0) : value(v), generation(1) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracker #" << n_created
            << ", created generation " << generation
            << " (total: " << n_created - n_destroyed << ")\n";
    }

    SortTracer(SortTracer const& b) : value(b.value), generation(b.generation) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracker #" << n_created
                  << ", copied as generation " << generation
                  << " (total: " << n_created - n_destroyed << ")\n";
    }

    ~SortTracer() {
        ++n_destroyed;
        update_max_live();
        std::cerr << "SortTracker generation " << generation
                  << " destroyed (total: " << n_created - n_destroyed << ")\n";
    }

    SortTracer& operator=(SortTracer const& b) {
        ++n_assigned;
        update_max_live();
        std::cerr << "SortTracker assignment #" << n_assigned
                  << " (generation " << generation
                  << " = " << b.generation << ")\n";
        value = b.value;
        return *this;
    }

    friend bool operator < (SortTracer const& a, SortTracer const& b) {
        ++n_compared;
        std::cerr << "SortTracker comparison #" << n_compared
                  << " (generation " << a.generation
                  << " < " << b.generation << ")\n";
        return a.value < b.value;
    }

    int val() const {
        return value;
    }
};

#endif //MODERN_CPP_DEMO_TRACKER_HPP

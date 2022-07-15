#include "gtest/gtest.h"

#include <condition_variable>
#include <chrono>
#include <mutex>

TEST(IfInitTest, IfInitTest) {
    std::mutex mutex;
    bool shared_flag = true;
    if(std::lock_guard<std::mutex> lock(mutex); shared_flag) {
        shared_flag = false;
    }
    ASSERT_FALSE(shared_flag);
}

TEST(SwitchInitTest, SwitchInitTest) {
    std::condition_variable cv;
    std::mutex cv_m;
    using namespace std::chrono_literals;
    switch(std::unique_lock<std::mutex> lock(cv_m); cv.wait_for(lock, 100ms)) {
        case std::cv_status::timeout:
            break;
        case std::cv_status::no_timeout:
            break;
    }
}
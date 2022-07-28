#include "gtest/gtest.h"

#include "folly/executors/ThreadedExecutor.h"
#include "folly/futures/Future.h"

#include <future>

TEST(FollyTest, FutureTest) {
    folly::ThreadedExecutor executor;
    printf("making Promise\n");
    folly::Promise<int> promise;
    folly::Future<int> f = promise.getSemiFuture().via(&executor);
    ASSERT_FALSE(f.isReady());
    auto f2 = std::move(f).thenValue([](int x) {
        printf("thenValue x(%d)\n", x);
        return x;
    });
    printf("Future chain made\n");

    printf("fulfilling Promise\n");
    promise.setValue(42);
    ASSERT_FALSE(f2.isReady());
    auto x = std::move(f2).get();
    ASSERT_EQ(42, x);
    printf("Promise fulfilled\n");
}

TEST(StlTest, FutureTest) {
    std::future<int> result = std::async([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 8;
    });

    ASSERT_EQ(8, result.get());

    // https://en.cppreference.com/w/cpp/thread/future
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t([&p]{
        try {
            // code that may throw
            throw std::runtime_error("Example");
        } catch(...) {
            try {
                // store anything thrown in the promise
                p.set_exception(std::current_exception());
            } catch(...) {} // set_exception() may throw too
        }
    });

    ASSERT_ANY_THROW(f.get());
    t.join();
}
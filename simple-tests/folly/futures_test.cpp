#include "gtest/gtest.h"

#include "folly/executors/ThreadedExecutor.h"
#include "folly/futures/Future.h"

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
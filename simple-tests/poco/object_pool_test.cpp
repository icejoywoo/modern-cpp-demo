#include "gtest/gtest.h"

#include "Poco/ObjectPool.h"

TEST(PocoTest, ObjectPoolTest) {
    struct Foo {
        int bar;
    };

    // ObjectPool(std::size_t capacity, std::size_t peakCapacity)
    Poco::ObjectPool<Foo> objectPool(10, 20);
    ASSERT_EQ(10, objectPool.capacity());
    ASSERT_EQ(20, objectPool.peakCapacity());

    Foo* f = objectPool.borrowObject();
    ASSERT_EQ(1, objectPool.size());
    objectPool.returnObject(f);
    Foo* f2 = objectPool.borrowObject();
    // same pointer
    ASSERT_EQ(f, f2);
    objectPool.returnObject(f2);

    Foo* fooArray[30];
    for (int i = 0; i < 30; i++) {
        fooArray[i] = objectPool.borrowObject();
    }
    ASSERT_EQ(20, objectPool.size());
    ASSERT_EQ(nullptr, fooArray[20]);

    for (int i = 0; i < 30; i++) {
        if (fooArray[i] != nullptr) {
            objectPool.returnObject(fooArray[i]);
        }
    }

    ASSERT_EQ(20, objectPool.available());
}
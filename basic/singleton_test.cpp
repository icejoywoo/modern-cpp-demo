#include "gtest/gtest.h"

#include <memory>

template<typename T>
class SingletonBase {
public:
    // arrow & velox singleton implementation
    static std::shared_ptr<T> &GetInstance() {
        static std::shared_ptr<T> instance(new T);
        return instance;
    }

private:
    SingletonBase(const SingletonBase &) = delete;

    SingletonBase(SingletonBase &&) = delete;

    SingletonBase &operator=(const SingletonBase &) = delete;

    SingletonBase &operator=(SingletonBase &&) = delete;

protected:
    SingletonBase() = default;

    virtual ~SingletonBase() = default;
};

#define SINGLETON_DECLARE(T) \
private:                     \
    friend class SingletonBase<T>; \
    T() = default;

class Singleton : public SingletonBase<Singleton> {
SINGLETON_DECLARE(Singleton);
public:
    constexpr int Get() {
        return 42;
    }
};

TEST(SingletonTest, BasicTest) {
    // singleton test
    ASSERT_EQ(Singleton::GetInstance(), Singleton::GetInstance());
    ASSERT_EQ(42, Singleton::GetInstance()->Get());
}
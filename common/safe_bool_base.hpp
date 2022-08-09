#pragma once

// https://www.artima.com/articles/the-safe-bool-idiom
// https://zhuanlan.zhihu.com/p/30173442
class SafeBoolBase {
 public:
  struct Dummy { void TruthValue(); };
  typedef void (Dummy::*SafeBoolType)();

 protected:
  // Disallow directly instantiating `SafeBoolBase`
  SafeBoolBase() {};
  SafeBoolBase(const SafeBoolBase&) {}

  // Disallow assigning through `SafeBoolBase*`.
  // For example,
  // Base *b1 = new Derived(), *b2 = new Derived();
  // *b1 = *b2;
  SafeBoolBase& operator= (const SafeBoolBase&) { return *this; }

  // Disallow deleting/destroying subclass objects through `SafeBoolBase*`.
  // For example,
  // Base *b = new Derived(); // use b
  // delete b; // Here's the problem!
  // Also Disallow directly instantiating `SafeBoolBase`
  ~SafeBoolBase() {};
};

// For testability without virtual function.
template<typename T=void>
class SafeBool: private SafeBoolBase {
  // private or protected inheritance is very important here
  // as it triggers the access control violation
  // for `ThisTypeDoesNotSupportComparisons`
  // 这种方法，对于不支持EBO (Empty Base Optimization)的编译器
  // 可能有size penalty
 public:
  operator SafeBoolType() const {
    return static_cast<const T*>(this)->BooleanTest() ?
        &Dummy::TruthValue : 0;
  }

 protected:
  SafeBool() {}
  SafeBool(const SafeBool&) {}
  SafeBool& operator= (const SafeBool&) { return *this; }
  ~SafeBool() {}
};

// For testability with a virtual function.
template<>
class SafeBool<void>: private SafeBoolBase {
 public:
  operator SafeBoolType() const {
    return BooleanTest() ? &Dummy::TruthValue : 0;
  }

 protected:
  virtual bool BooleanTest() const = 0;
  virtual ~SafeBool() {};

 protected:
  SafeBool& operator= (const SafeBool&) { return *this; }
};

template<typename U, typename V>
bool operator== (const SafeBool<U>& lhs, const SafeBool<V>& rhs) {
  lhs.ThisTypeDoesNotSupportComparisons();
  return false;
}

template<typename U, typename V>
bool operator!= (const SafeBool<U>& lhs, const SafeBool<V>& rhs) {
  lhs.ThisTypeDoesNotSupportComparisons();
  return false;
}

#include "gtest/gtest.h"
#include <boost/intrusive_ptr.hpp>

#include <cassert>
#include <atomic>
#include <iostream>

class ReferenceCounter
{
 public:
  friend void intrusive_ptr_add_ref(ReferenceCounter *p)
  {
    std::cout << "Call intrusive_ptr_add_ref" << std::endl;
    assert(p);
    assert(p->ref_count >= 0);
    p->ref_count++;
    p->add_ref_count++;
  }

  friend void intrusive_ptr_release(ReferenceCounter *p)
  {
    std::cout << "Call intrusive_ptr_release" << std::endl;
    assert(p);
    assert(p->ref_count > 0);
    p->release_count++;
    if (--p->ref_count == 0)
    {
      delete p;
    }
  }

  ReferenceCounter() :ref_count(0)
  {
    std::cout << "Reference Counter Constructor" << std::endl;
    this->constructor_count++;
  }

  ReferenceCounter(const ReferenceCounter  &other)
  {
    std::cout << "Reference Counter Copy Constructor" << std::endl;
    this->copy_count++;
  }

  ReferenceCounter& operator=(const ReferenceCounter &other)
  {
    std::cout << "Reference Counter Assignment Operator" << std::endl;
    this->assignment_count++;
    return *this;
  }

  ~ReferenceCounter()
  {
    std::cout << "Reference Counter Destructor" << std::endl;
  };

  int RefCount()
  {
    return ref_count;
  }

 private:
  std::atomic_int ref_count;
  std::atomic_int add_ref_count;
  std::atomic_int release_count;
  std::atomic_int constructor_count;
  std::atomic_int copy_count;
  std::atomic_int assignment_count;
};

class ProcessData : public ReferenceCounter
{
 public:
  ProcessData(int id, const std::string&& info) :m_id(id), m_info(info)
  {
    std::cout << "Process Data Constructor" << std::endl;
  }

  ProcessData(const ProcessData &other)
   : ReferenceCounter(other) {
    std::cout << "Process Data Copy Constructor" << std::endl;
    m_id = other.m_id;
    m_info = other.m_info;
  }

  ProcessData& operator=(const ProcessData &other)
  {
    std::cout << "Process Data Assignment Operator" << std::endl;
    m_id = other.m_id;
    m_info = other.m_info;
    return *this;
  }

  ~ProcessData()
  {
    std::cout << "Process Data Destructor" << std::endl;
  }

 private:
  int m_id;
  std::string m_info;
};

TEST(BoostTest, IntrusivePtrTest) {
  // https://www.jianshu.com/p/dd9701219b27
  boost::intrusive_ptr<ProcessData> ptr(new ProcessData(1, "a"));
  EXPECT_EQ(1, ptr->RefCount());

  {
    // shared_ptr 不可以创建多个，会导致 double free 的问题
    // copy
    boost::intrusive_ptr<ProcessData> copy_ptr(ptr.get());
    EXPECT_EQ(2, ptr->RefCount());
    EXPECT_EQ(2, copy_ptr->RefCount());
  }

  EXPECT_EQ(1, ptr->RefCount());

  {
    // assignment
    boost::intrusive_ptr<ProcessData> assign_ptr = ptr;
    EXPECT_EQ(2, ptr->RefCount());
    EXPECT_EQ(2, assign_ptr->RefCount());
  }

  EXPECT_EQ(1, ptr->RefCount());

  {
    // weak
    boost::intrusive_ptr<ProcessData> weak_ptr(ptr.get(), false);
    EXPECT_EQ(1, ptr->RefCount());
    EXPECT_EQ(1, weak_ptr->RefCount());
  }
  // ptr is destructed
}

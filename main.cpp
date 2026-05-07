#include <iostream>
#include <type_traits>
#include <array>
#include "CommonIncludes.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.


template<typename T>
typename std::remove_reference<T>::type&& move_me_old(T&& val) noexcept
{
  return static_cast<typename std::remove_reference<T>::type&&>(val);
}

template<typename T>
auto move_me_new(T&& val) noexcept -> typename std::remove_reference_t<decltype(val)>&&
{
  return static_cast<typename std::remove_reference_t<decltype(val)>&&>(val);
}

struct A
{
  double d;
  int i;
  long l;

  A() = default;

  A(double d_, int i_, long l_) : d(d_), i(i_), l(l_) {
    std::cout << "Inside the constructor" << std::endl;
  }

  A(const A& obj) {
    d = obj.d;
    i = obj.i;
    l = obj.l;
    std::cout << "Inside the copy constructor" << std::endl;
  }

  A(A&& obj) {
    d = obj.d;
    i = obj.i;
    l = obj.l;
    std::cout << "Inside the move constructor" << std::endl;
  }

  A& operator=(const A& obj)
  {
    d = obj.d;
    i = obj.i;
    l = obj.l;
    std::cout << "Inside the copy assignment operator" << std::endl;

    return *this;
  }

  A&& operator=(const A&& obj)
  {
    d = obj.d;
    i = obj.i;
    l = obj.l;
    std::cout << "Inside the move assignment operator" << std::endl;

    return static_cast<A&&>(*this);
  }

  ~A() = default;
};

int main()
{
  // Structured Binding Tests
  {
    cppprep::cppfeatures::StructuredBindingTest sbt;
    sbt.run_test();
  }

  // Tuple Tests
  {
    cppprep::cppfeatures::TupleTest tt;
    tt.run_test();
  }

  // std::invoke tests
  {
    cppprep::cppfeatures::InvokeTest inv;
    inv.run_test();
  }

  return 0;
}
#pragma once
#include <iostream>

namespace cppprep::metaprogramming
{
  template<unsigned int x, unsigned int y>
  struct GCD
  {
    static const unsigned int result = GCD<y, x%y>::result;
  };

  template<unsigned int x>
  struct GCD<x, 0>
  {
    static const unsigned int result = x;
  };

  consteval  unsigned int gcd_calc(const unsigned int x, const unsigned int y) noexcept{

    return (y==0)? x : gcd_calc(y, x%y);
  }


  class GCD
  {
    public:
      void run_test() {
        std::cout << GCD<11u, 5u>::result << std::endl;
        std::cout << GCD<40u, 10u>::result << std::endl;

        std::cout << "The GCD of 11 and 5 is: " << gcd_calc(11u, 5u) << std::endl;

      }
  };

}

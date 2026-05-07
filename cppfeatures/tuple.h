#pragma once
#include <iostream>
#include <ostream>
#include <array>
#include <vector>
#include <tuple>

/**
 * std::tuple
 *
 *
 * std::apply
 * std::apply is a utility introduced in C++17 that allows you to unpack a tuple-like object (like a std::tuple,
 * std::pair, or std::array) and pass its elements as individual arguments into a function.
 *
 * Think of it as the bridge between compile-time structured data (tuples, pairs, arrays) and standard function
 * invocation.
 *
 * The Problem It Solves
 * If you have a function that expects individual arguments, you cannot pass it a tuple directly:
 * e.g.
 * void process_order(double price, int quantity, char side);
 * std::tuple<double, int, char> tick = {105.25, 500, 'B'};
 * // process_order(tick); // Compilation Error! It expects 3 args, not 1 tuple.
 *
 * std::apply automates this cleanly under the hood:
 * std::apply(process_order, tick);
 */

namespace cppprep::cppfeatures
{

  auto add_lambda = [](auto first, auto second) {
    return first + second;
  };

  template<typename... Ts>
  std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple)
  {
    std::apply
    (
        [&os](Ts const&... tupleArgs)
        {
            os << '[';
            std::size_t n{0};
            ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
  }

  class Tuple
  {
      public:
        void withApplyToExpandTuple() {
          std::pair<int, int> inputs{5,10};
          std::cout << std::apply(add_lambda, inputs) << std::endl;
        }

        void withArray() {
          std::array<int, 5> arr = {10, 20, 30, 40, 50};
          auto tp = std::apply([](auto... elems) {
             return std::make_tuple(elems...);
          }, arr);

          std::cout << tp << std::endl;
        }

        void withTie() {
          std::array<int,3> arr = {1,2,3};

          int arg1, arg3;
          std::tie(arg1, std::ignore, arg3) = arr;

          std::cout << "arg1 : " << arg1 << "; arg3 : " << arg3 << std::endl;
        }
  };


  class TupleTest
  {
    public:
      void run_test()
      {
        Tuple t{};

        t.withApplyToExpandTuple();
        t.withArray();
        t.withTie();
      }
  };


}
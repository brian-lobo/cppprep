#pragma once
#include <iostream>
#include <ostream>
#include <array>
#include <tuple>

/**
 * STRUCTURED BINDINGS
 *
 * Introduction
 * They’re a zero-overhead abstraction that improves readability in hot paths (market-data parsing, order-book updates,
 * risk structs, etc.) without costing cycles. Firms expect you to know exactly how they desugar, the reference
 * semantics, lifetime rules, custom-type support, and the C++26 pack/attribute extensions (especially in 2026).
 *
 * Core Concept and Syntax
 *
 * Structured bindings let you decompose an object into named variables in one declaration. Think “std::tie on
 * steroids that actually declares the variables.”
 *
 * e.g.
 * // Basic tuple
 * struct MarketTick
 * {
 *     double d;
 *     double quantity;
 *     double side;
 * };
 *
 * MarketTick mt{};
 * auto [price, qty, side] = mt;  // C++17+
 *
 * // With qualifiers (critical for low-latency)
 * const auto& [bid, ask, ts] = order_book_snapshot();  // reference binding
 * auto&& [px, vol] = std::move(temporary_tick());     // forwarding reference
 *
 * // With C Array/std::array
 * char arr[5] = {1,2,3,4,5 };
 * auto [a, b, c, d, e] = arr;
 *
 * // using std::tie
 * char arr1[5];
 * std::tie(&arr1) = arr;
 *
 * Under The Hood
 * --------------
 * 01. The compiler does not create multiple independent variables. It creates one hidden object and binds references to
 * its subobjects. Equivalent code the compiler generates (simplified):
 *
 * auto __e = expr;                     // temporary if rvalue
 * auto& v0 = get<0>(__e);              // or __e.member0 for aggregates
 * auto& v1 = get<1>(__e);              // ...
 * // your bindings are just aliases to these references
 *
 * 02. Zero cost: No extra instructions vs. manual member access (check on Godbolt.org with -O3 -std=c++23). You’ll
 * see the exact same assembly as double px = tick.px;.
 *
 * 03. Lifetime extension: If expr is a prvalue (temporary), the hidden object lives as long as the bindings stay in
 * scope—same rule as range-based for with temporaries.
 *
 * 04. References are to the hidden object’s members, so mutating a binding mutates the original
 *
 * Key Semantics & Gotchas
 * -----------------------
 *
 * 01. Not std::tie: std::tie assigns to existing variables. Structured bindings declare new ones. Huge difference
 *     in hot loops.
 *
 * 02. No bit-fields directly (pre-C++26 aggregates with bit-fields are restricted; use a getter or tuple-like wrapper).
 *
 * 03. Name lookup: The bindings are in the surrounding scope. No re-declaration in the same scope.
 *
 * 04. Lambda capture: Pre-C++26 you couldn’t directly capture a structured binding by value easily (had to capture
 *     the whole object or use init-capture). C++26 pack bindings help.
 *
 * 05. Move semantics: auto [a, b] = std::move(t); moves from the hidden temporary—perfect for ownership transfer in
 *     order routers.
 *
 * 06. constexpr: Fully supported
 *
 * Performance in HFT context
 * --------------------------
 * 01. Zero measurable overhead on any modern compiler (GCC/Clang/MSVC). Use them freely in market-data hot paths instead
 *     of manual unpacking or std::tie. They encourage data-oriented design (unpack once, then process).
 *
 * 02. Common real-world HFT patterns
 *     for (const auto& [symbol, px, qty, ts] : multicast_batch) {
 *       if (px > threshold) update_order_book(symbol, px, qty);
 *     }
 *     Cleaner than five times data[0], data[1], etc., and just as fast
 *
 * HFT Interview Questions on Structured Bindings (Ready-to-Use Set)
 * -----------------------------------------------------------------
 * 10 questions from fundamentals to architecture/performance/trade-offs
 * 01. What are structured bindings, when were they introduced, and why are they better than std::tie?
 * 02. Basic coding
 *     You receive a std::tuple<double, int, char> from a market-data feed representing price, quantity, side. Write
 *     the cleanest C++17+ code to unpack it and log only if quantity > 1000. (Bonus: make it const auto&.)
 * 03. Real HFT scenario
 *     Design a struct MarketTick (price, size, timestamp, symbol_id) that supports structured bindings. Show two
 *     ways: as aggregate vs. tuple-like protocol. Which do you prefer in a 1μs hot path and why?
 * 04. Reference semantics
 *     What is the lifetime of the hidden object in auto [a, b] = make_temp_tuple();? What if you do
 *     auto& [a, b] = make_temp_tuple();? (Expect lifetime-extension discussion.)
 * 05. Performance deep-dive
 *     Does using structured bindings introduce any runtime overhead compared to manual member access in a
 *     lock-free order-book update loop? How would you prove it? (Answer: zero; show Godbolt equivalence.)
 * 06. C++26 awareness (shows you’re current)
 *     C++26 added pack bindings and per-binding attributes. Give an example where auto [...fields] = msg; would
 *     simplify a generic FIX decoder, and how you’d annotate one field with [[maybe_unused]].
 * 07. Gotcha / UB trap
 *     Can you bind to a bit-field member? What happens if you try? How do you work around it in a risk struct that
 *     uses bit-fields for flags?
 * 08. Design / maintainability
 *     Your team has a legacy market-data parser using raw arrays and manual indexing. Refactor it to use structured
 *     bindings + aggregates. What trade-offs do you discuss with the quant team?
 * 09. Advanced / templates
 *     Write a template function that takes any tuple-like object and uses structured bindings inside to extract the
 *     first two elements. What limitations exist pre-C++26?
 * 10. Production war story (behavioral + technical)
 *     Tell me about a time you introduced (or removed) structured bindings in a latency-critical path. What was the
 *     measurable impact on readability, compile time, and nanosecond latency?
 */

namespace cppprep::cppfeatures
{
  static struct MarketTick {
    double price;
    double quantity;
    double side;
  }mt;

  class StructuredBinding
  {
      public:
        void withStructures() {
          // using structures
          MarketTick mtOrder[] {
            {1.0, 10.0, 1},
            {2.0, 20.0, 0},
            {3.0, 30.0, 1},
            {4.0, 40.0, 0},
            {5.0, 50.0, 1}
          };
          for (const auto& [price, quantity, side] : mtOrder){
            std::cout << price << " " << quantity << " " << side << std::endl;
          }
        }

        void withArray() {
          // Using std::array
          std::array<int, 5> nums = {1,2,3,4,5 };
          [[maybe_unused]] auto [_, b, c, d, e] = nums;

          std::cout << b << " " << c << " " << d << " " << e << std::endl;
        }

        void withTie() {
          std::array<int, 5> nums = {6,7,8,9,10 };
          int b, c, d,e;
          std::tie(std::ignore, b, c, d, e) = nums;

          std::cout << "_ (ignored)" << b << " " << c << " " << d << " " << e << std::endl;
        }
  };


  class StructuredBindingTest
  {
    public:
      void run_test()
      {
        StructuredBinding sb{};

        sb.withStructures();
        sb.withArray();
        sb.withTie();
      }
  };


}
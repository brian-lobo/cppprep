#pragma once

#include <iostream>
#include <functional>

/**
 * if constexpr
 *
 * Lets the compiler discard entire branches at compile time, eliminating runtime branches, dead code, and often
 * massive compile-time complexity.
 *
 * template <typename T>
 * void process_tick(const T& tick) {
 *   if constexpr (std::is_same_v<T, EquityTick>) {
 *       handle_equity(tick);          // Only this branch exists for EquityTick
 *   } else if constexpr (std::is_same_v<T, OptionTick>) {
 *       handle_option(tick);
 *   } else {
 *       static_assert(false, "Unsupported tick type");  // Only evaluated if reached
 *   }
 * }
 *
 * The condition must be a constant expression (contextually converted to bool).
 * Only the taken branch is instantiated/compiled. The discarded branch is completely ignored—no ODR-use, no
 * instantiation of templates inside it, no compilation errors from invalid code in the false branch.
 * This is not a regular if with optimization. It’s a core language compile-time selection mechanism.
 *
 * Key differences from regular if:
 * Regular if: Both branches compiled; runtime branch (potential misprediction in HFT hot paths).
 * if constexpr: Compile-time only. Zero runtime cost. Discarded branch can contain ill-formed code (e.g., calling
 * non-existent members).
 *
 * 2. Major Use Cases in HFT/Low-Latency Systems
 *    Generic market-data handlers — Different parsers per venue/instrument type without virtuals or function pointers.
 *    Zero-overhead polymorphism — Replace CRTP + inheritance or type erasure in hot paths.
 *    Feature toggles at compile time — e.g., SIMD paths, debug vs prod risk checks, different order types.
 *    Avoiding runtime dispatch — Critical for nanosecond-level code where branch prediction misses cost dearly.
 *    Metaprogramming simplification — Replaces most SFINAE in C++17+ codebases.
 *
 * template <typename Msg>
 * void on_message(const Msg& m) {
 *    if constexpr (has_price<Msg>()) {          // Concept or trait
 *        update_order_book(m.symbol, m.price, m.qty);
 *    }
 *    if constexpr (is_risk_relevant<Msg>()) {
 *        update_pnl(m);
 *    }
 * }
 *
 * 3. Under the Hood & Compiler Behavior
 *    The compiler evaluates the condition during template instantiation.
 *    Discarded statements are not parsed beyond basic syntax (in practice, very little codegen).
 *    Works inside constexpr functions, templates, and non-template functions (as long as condition is constexpr).
 *
 *    Combine with concepts (C++20) for even cleaner code:
 *
 *    if constexpr (std::is_integral_v<T> && sizeof(T) == 8) { ... }
 *
 * 4. Gotchas, Limitations & Senior-Level Nuances

 *    Condition must be constexpr—runtime variables cause compile error.
 *    static_assert in discarded branch is not evaluated (very useful).
 *    Lambdas and local classes inside if constexpr have specific rules.
 *    Nested if constexpr works fine but can become hard to read (prefer concepts or if consteval in newer standards).
 *    ODR and linkage: Discarded branches don’t trigger ODR violations.
 *    Debugging: Harder because code literally doesn’t exist in one instantiation—use #ifdef or static_assert strategically.
 *    Pre-C++20: No concepts, so heavier reliance on std::enable_if + if constexpr.
 *    Compile times: Can increase if overused in deeply nested templates (trade-off discussion point)
 *
 * 5. Best Practices for Production Trading Systems
 *    - Prefer over SFINAE/tag dispatch for readability and maintainability.
 *    - Use with requires clauses and concepts for self-documenting code.
 *    - Measure: Always verify on Godbolt + microbenchmarks (Google Benchmark) that you eliminated branches.
 *    - In latency-critical paths: Combine with data-oriented design, custom allocators, and constexpr computations.
 *    - Avoid in very hot loops if the condition varies per instantiation (but that’s the point—different templates get
 *      specialized code).
 *
 * HFT Interview Questions on if constexpr
 * 01. Warm-up
 *     Explain if constexpr vs regular if. When was it introduced, and why is it useful?
 * 02. Basic coding
 *     Write a template function that processes different order types (MarketOrder, LimitOrder, IOC) using if
 *     constexpr. Only compile the relevant handling code.
 * 03. HFT scenario
 *     You have a generic market-data feed handler templated on venue (NYSE vs CME vs custom binary). How would you
 *     use if constexpr (and/or concepts) to select different parsing logic with zero runtime cost?
 * 04. Semantics deep-dive
 *     What happens if the discarded branch contains a static_assert(false) or a call to a non-existent member function?
 *     Why does this not cause a compile error?
 * 05. Performance / low-latency
 *     Does if constexpr eliminate branch prediction misses in a hot order-book update loop? How would you prove it?
 *     Compare to virtual functions or function pointers.
 * 06. Replacement for older idioms
 *     Show how if constexpr + concepts replaces std::enable_if / SFINAE in a trait-based risk calculator. What are the
 *     advantages?
 * 07. Gotcha / edge case
 *     Can you use a runtime variable in the condition? What about inside a constexpr function? Explain if consteval
 *     (C++23) and when you’d use it.
 * 08. C++26 awareness
 *     With expanded constexpr in C++26 (e.g., more containers, placement new), how might you use if constexpr in a
 *     compile-time order book simulator or static configuration?
 * 09. Design / trade-offs
 *     Your team’s legacy code uses heavy template specialization + tag dispatch. Refactor a piece using if constexpr.
 *     What readability, compile-time, and runtime trade-offs do you discuss?
 * 10. Production war story
 *     Tell me about a time you used (or removed) if constexpr in a latency-critical path. What was the measurable
 *     impact on nanoseconds, code size, or debuggability?
 *
 *
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * consteval
 *
 * To understand consteval (C++20) and constinit (C++20), it helps to look at them as sharper, more specialized tools
 * carved out of the original constexpr keyword.
 * While constexpr means "potentially compile-time," these two new keywords give you exact, deterministic control over
 * when code executes and how variables are initialized—which is critical for eliminating runtime latency and avoiding
 * the dreaded "static initialization order fiasco."
 *
 * 1. consteval — Immediate Functions (Guaranteed Compile-Time)
 *    The constexpr keyword tells the compiler: "Run this at compile time if you can, but if someone passes it a
 *    runtime variable, just compile it as a regular runtime function." consteval is much stricter. It specifies that
 *    a function must be evaluated at compile time. If the compiler cannot evaluate it at compile time, it throws a
 *    compilation error. Functions marked with consteval are called immediate functions.
 *
 *    #include <iostream>
 *    consteval int multiply(int a, int b) {
 *        return a * b;
 *    }
 *
 *    int main() {
 *        // 1. Valid: Arguments are known at compile time
 *        constexpr int static_val = multiply(10, 5);
 *        int normal_val = multiply(10, 5); // Still happens at compile time!
 *
 *        // 2. Compilation Error!
 *        int x = 10;
 *        // int bad_val = multiply(x, 5);
 *        // Error: 'x' is not a constant expression. consteval cannot be run at runtime.
 *    }
 *
 * Why use it in High-Performance Code?
 * Zero Runtime Footprint: Because a consteval function only executes during compilation, the function itself never
 * actually gets generated as assembly code in your final binary. There is no overhead, no function call, and no branch.
 * Safer Macros Replacement: You can use it to build type-safe compile-time string parsers, hash calculations (like
 * CRC32 or MurmurHash for trading symbols), or fixed-point math configurations without relying on dangerous C-style
 * preprocessor macros.
 *
 * constinit
 *
 * Guaranteed Static Initialization
 * In C++, global or static variables can be initialized in two ways:
 * Static Initialization: The compiler calculates the value and bakes it directly into the binary's data segment. It
 * is ready before main() even starts.
 *
 * Dynamic Initialization: The value must be computed at runtime when the program boots up (e.g., calling a function
 * to initialize a global variable).
 *
 * If you have multiple global variables across different files that depend on each other, the order in which dynamic
 * initialization happens is undefined. This leads to the Static Initialization Order Fiasco (crashing because a global
 * variable tries to read another global variable that hasn't booted up yet).
 *
 * constinit forces the compiler to guarantee that a variable undergoes static initialization. If the initializer
 * requires a runtime computation, the compiler rejects it.
 *
 *     #include <array>
 *     int get_runtime_value() { return 42; }
 *     constexpr int get_compile_value() { return 100; }
 *
 *     // 1. Valid: Value is computed entirely at compile time
 *     constinit int global_factor = get_compile_value();
 *
 *     // 2. Compilation Error!
 *     // constinit int bad_global = get_runtime_value();
 *     // Error: variable does not have a constant initializer
 *
 *     int main() {
 *         // 3. Compilation Error!
 *         // constinit int local_var = 5;
 *         // Error: constinit can only be applied to variables with static or thread storage duration.
 *     }
 *
 * Crucial Note: constinit variables are MUTABLE!
 * Unlike constexpr, which forces a variable to become const (read-only), constinit does not make the variable constant.
 * It only dictates how it starts its life. Once main() begins, your program can freely modify a constinit variable.
 *
 * Comparison Cheat SheetKeywordCan be applied to...Is the value read-only (const)?EnforcementconstexprFunctions & VariablesYes (for variables)Optional/Conditional: Run at compile time if possible.constevalFunctions onlyN/AMandatory: Must execute at compile time, or compile fails.constinitStatic/Global Variables onlyNo (it remains mutable)Mandatory: Must be initialized at compile time, or compile fails.
 */

namespace cppprep::cppfeatures
{
  class IfConstExpr
  {
      public:


  };


  class IfConstExprTest
  {
    public:
      void run_test()
      {

      }
  };
}
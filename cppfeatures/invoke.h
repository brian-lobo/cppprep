#pragma once

#include <iostream>
#include <functional>

/**
 * std::invoke
 *
 * std::invoke is a standard library utility introduced in C++17 (defined in the <functional> header) that provides a
 * uniform way to call any "Callable" object.
 * Before std::invoke, C++ had inconsistent syntax for calling different types of things. A regular function is called
 * with func(), a function pointer with (*func_ptr)(), a lambda with lambda(), and a member function pointer with
 * (obj.*mem_func_ptr)().
 *
 * std::invoke unifies all of this under a single, predictable syntax:
 *
 * std::invoke(callable, arguments...).
 *
 * Why Do We Need It? (The Problem)
 *
 * Imagine you are writing a generic template function that needs to execute a callback passed by a user. If you write
 * it the traditional way, your code will fail if the user passes a pointer to a member function:
 *
 * template <typename F, typename Arg>
 * void executeCallback(F f, Arg arg) {
 *   f(arg); // ❌ Throws a compiler error if 'f' is a member function pointer!
 *           // Member functions require syntax like: (object.*f)(arg)
 * }
 *
 * std::invoke completely solves this problem. It handles the internal compiler logic to determine how the object needs
 * to be called.
 *
 *
 */

namespace cppprep::cppfeatures
{
  void printSum(int a, int b) {
    std::cout << "Sum: " << a + b << "\n";
  }

  // A generic function that applies a callable to every element in a container
  template <typename Container, typename Callable>
  void mapOver(const Container& container, Callable f) {
    for (const auto& item : container) {
      // Securely invokes free functions, lambdas, or member variables!
      std::cout << std::invoke(f, item) << " ";
    }
    std::cout << "\n";
  }

  class Greeter {
  public:
    void sayHello(const std::string& name) const {
      std::cout << "Hello, " << name << "!\n";
    }
  };

  struct User {
    std::string name;
  };

  struct Player {
    std::string name;
    int score;
  };

  class Invoke
  {
      public:
        void invokeFreeFunctionsAndFunctionPointers() {
          // Direct function invocation
          std::invoke(printSum, 10, 5); // Outputs: Sum: 15

          // Function pointer invocation
          void (*ptr)(int, int) = printSum;
          std::invoke(ptr, 20, 10);     // Outputs: Sum: 30
        }

        void invokeLambdasAndFunctors() {
          auto multiply = [](int a, int b) { return a * b; };

          // Invoking a lambda
          int result = std::invoke(multiply, 6, 7);
          std::cout << "Product: " << result << "\n"; // Outputs: 42
        }

        void invokeMemberFunctions() {
          Greeter greeter;

          // Notice the syntax: &ClassName::MethodName, followed by the object instance
          std::invoke(&Greeter::sayHello, greeter, "Alice"); // Outputs: Hello, Alice!

          // It also works perfectly if you pass a pointer to the object
          Greeter* greeterPtr = &greeter;
          std::invoke(&Greeter::sayHello, greeterPtr, "Bob"); // Outputs: Hello, Bob!
        }

        void accessingDataMembers() {
          Player p1{"Hero", 999};

          // Acts as a getter for the 'score' variable
          int currentScore = std::invoke(&Player::score, p1);
          std::cout << "Score: " << currentScore << "\n"; // Outputs: 999
        }

        void writingPerfectGenericTemplates() {
          std::vector<User> users{ {"Alice"}, {"Bob"}, {"Charlie"} };

          // We pass a pointer to a data member directly into our generic template!
          mapOver(users, &User::name); // Outputs: Alice Bob Charlie
        }

        void invokeReturn() {
          auto add = [](int x, int y) { return x + y; };

          // Forces the integer result (33) to be converted and returned as a float
          float ret = std::invoke_r<float>(add, 11, 22);

          std::cout << std::fixed << ret << "\n"; // Outputs: 33.000000
        }
  };


  class InvokeTest
  {
    public:
      void run_test()
      {
        Invoke inv{};

        inv.invokeFreeFunctionsAndFunctionPointers();
        inv.invokeLambdasAndFunctors();
        inv.invokeMemberFunctions();
        inv.accessingDataMembers();
        inv.writingPerfectGenericTemplates();
        inv.invokeReturn();
      }
  };
}
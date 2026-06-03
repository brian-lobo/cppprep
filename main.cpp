#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "leetcode/fractiontorecurringdecimal.h"
#include "leetcode/medianoftwosortedarrays.h"
#include <vector>
#include <set>

#include "leetcode/dfs/numberofislands.h"

using namespace std;


#include <vector>
#include <deque>
using namespace std;

class MaxSlidingWindowSolution {
public:
  vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> result;
    deque<int> dq;        // Stores indices in decreasing order of values

    for (int i = 0; i < nums.size(); ++i) {

      // Step 1: Remove elements out of current window
      if (!dq.empty() && dq.front() == i - k) {
        dq.pop_front();
      }

      // Step 2: Remove smaller elements from back (they can't be max anymore)
      while (!dq.empty() && nums[dq.back()] < nums[i]) {
        dq.pop_back();
      }

      // Step 3: Add current index
      dq.push_back(i);

      // Step 4: Add maximum to result when window is full
      if (i >= k - 1) {
        result.push_back(nums[dq.front()]);
      }
    }

    return result;
  }
};

int main() {

  leetcode::dfs::NumberOfIslandsSolutionTest sol;
  sol.run_test();

  MaxSlidingWindowSolution s;

  {
    std::vector<int> nums{1,3,-1,-3,5,3,6,7};
    int k{3};

    s.maxSlidingWindow(nums, k);
  }


  return 0;
}


// #include <iostream>
// #include <type_traits>
// #include <array>
// #include "CommonIncludes.h"
// #include <memory>
// #include <queue>
// #include <mutex>
// #include <thread>
// #include <chrono>
// #include <semaphore>
// #include <bit>
// #include <bitset>
// #include <memory>
// #include <chrono>
// #include <iterator>
// #include <sstream>
// #include <unordered_set>
// #include <unordered_map>
// #include <set>
// #include <vector>
// // TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
//
//
// template<typename T>
// typename std::remove_reference<T>::type&& move_me_old(T&& val) noexcept
// {
//   return static_cast<typename std::remove_reference<T>::type&&>(val);
// }
//
// template<typename T>
// auto move_me_new(T&& val) noexcept -> typename std::remove_reference_t<decltype(val)>&&
// {
//   return static_cast<typename std::remove_reference_t<decltype(val)>&&>(val);
// }
//
// struct A
// {
//   double d;
//   int i;
//   long l;
//
//   A() = default;
//
//   A(double d_, int i_, long l_) : d(d_), i(i_), l(l_) {
//     std::cout << "Inside the constructor" << std::endl;
//   }
//
//   A(const A& obj) {
//     d = obj.d;
//     i = obj.i;
//     l = obj.l;
//     std::cout << "Inside the copy constructor" << std::endl;
//   }
//
//   A(A&& obj) {
//     d = obj.d;
//     i = obj.i;
//     l = obj.l;
//     std::cout << "Inside the move constructor" << std::endl;
//   }
//
//   A& operator=(const A& obj)
//   {
//     d = obj.d;
//     i = obj.i;
//     l = obj.l;
//     std::cout << "Inside the copy assignment operator" << std::endl;
//
//     return *this;
//   }
//
//   A&& operator=(const A&& obj)
//   {
//     d = obj.d;
//     i = obj.i;
//     l = obj.l;
//     std::cout << "Inside the move assignment operator" << std::endl;
//
//     return static_cast<A&&>(*this);
//   }
//
//   ~A() = default;
// };
//
//
//
// template<class ForwardIt, class T = typename std::iterator_traits<ForwardIt>::value_type>
// ForwardIt remove_elements(ForwardIt first, ForwardIt last, const T& value)
// {
//   first = std::find(first, last, value);
//   if (first != last)
//     for (ForwardIt i = first; ++i != last;)
//       if (!(*i == value))
//         *first++ = std::move(*i);
//   return first;
// }
//
// template<typename T>
// class TestUniversalReference
// {
//     public:
//         template<typename T2, typename U>
//         void foo(T2&& t, U&& u) {
//           std::cout << "determine the types" << std::endl;
//         }
// };
//
//
// class Test
// {
//     Test* _instance{nullptr};
//     std::once_flag flag;
//   public:
//     Test* getInstance() {
//       std::call_once(flag, [&]() {
//         _instance = new Test();
//       });
//       return _instance;
//     }
// };
//
// uint32_t reverseBits(uint32_t n) {
//   n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
//   n = ((n & 0xff00ff00) >> 8)  | ((n & 0x00ff00ff) << 8);
//   n = ((n & 0xf0f0f0f0) >> 4)  | ((n & 0x0f0f0f0f) << 4);
//   n = ((n & 0xcccccccc) >> 2)  | ((n & 0x33333333) << 2);
//   n = ((n & 0xaaaaaaaa) >> 1)  | ((n & 0x55555555) << 1);
//   return n;
// }
//
// class OrderStrategy : public std::enable_shared_from_this<OrderStrategy> {
// private:
//   // 1. Private constructor prevents direct stack/heap creation
//   OrderStrategy() = default;
//
//   // 2. Separate initialization step for async hooks
//   void initialize() {
//     // SAFE: The factory function guarantees a shared_ptr owns 'this' now
//     auto self = shared_from_this();
//     // register_callback(self);
//   }
//
// public:
//   // 3. Force creation through a unified factory
//   static std::shared_ptr<OrderStrategy> create() {
//     // Heap allocation and control block creation happen simultaneously
//     auto instance = std::make_shared<OrderStrategy>(OrderStrategy{});
//
//     // Safely initialize components requiring shared_from_this
//     instance->initialize();
//
//     return instance;
//   }
// };
//
// void f(const std::string & s) {}
//
// #pragma pack(push, 1)
// struct PackSample
// {
//   char ch{};
//   int n{};
//   double d{};
// };
// #pragma (pop)
//
// struct PackSampleNoPack
// {
//   char ch{};
//   int n{};
//   double d{};
// };
//
// void textJustification(std::vector<std::string> words) {
//
//
//
//   int maxSize=16;
//   std::vector<std::string> result{};
//   int cnt = 0;
//   std::string placeholder{""};
//   for (const std::string& word : words)
//   {
//     if (placeholder.length() + 1 + word.length() > maxSize)
//     {
//       while (placeholder.length() != maxSize) placeholder.append(" ");
//       result.push_back(placeholder);
//       placeholder=word;
//     }
//     else
//     {
//       if (placeholder.length()!=0) placeholder.append(" ");
//       placeholder.append(word);
//     }
//   }
//   if (placeholder.length()> 0) result.push_back(placeholder);
//
//   // for (auto line: result)
//   //    std::cout << line << std::endl;
//
//   std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout,"\n"));
// }
//
//
//
// class Subsets {
//   public:
//     std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
//         std::vector<std::vector<int>> result;
//         std::vector<int> path;
//
//         backtrack(nums, 0, path, result);
//         return result;
//     }
//
//     void backtrack(const std::vector<int>& nums,
//                int start,
//                std::vector<int>& path,
//                std::vector<std::vector<int>>& result) {
//       // Every state we reach is a valid subset
//       result.push_back(path);           // copy current path
//
//       // Try including each remaining element
//       for (int i = start; i < nums.size(); ++i) {
//         path.push_back(nums[i]);      // make choice
//         backtrack(nums, i + 1, path, result);
//         path.pop_back();              // backtrack (undo)
//       }
//     }
// };
//
// class WordSearch {
//   public:
//     bool exist(std::vector<std::vector<char>>& board, std::string word) {
//       std::unordered_map<char, int> count{};
//       for (int row=0; row < board.size(); row++)
//       {
//         for (int col=0; col < board.size(); col++)
//         {
//
//         }
//       }
//     }
// };
//
//
// class Solution {
// public:
//   bool checkValid(std::vector<std::vector<int>>& matrix) {
//     int n = matrix.size();
//
//     for (int row = 0; row < n; row++) {
//       std::vector<bool> rowCheck(n + 1, false);
//       std::vector<bool> colCheck(n + 1, false);
//
//       for (int val = 0; val < n; val++) {
//         // Check row i
//         int rowVal = matrix[row][val];
//         if (rowCheck[rowVal]) return false; // Duplicate found
//         rowCheck[rowVal] = true;
//
//         // Check column i
//         int colVal = matrix[val][row];
//         if (colCheck[colVal]) return false; // Duplicate found
//         colCheck[colVal] = true;
//       }
//     }
//
//     return true;
//   }
// };
//
//
//
//
//
// int main()
// {
//   designpatterns::structural::DecoratorTest dt;
//   dt.run_test();
//
//   return 0;
//
//   {
//     std::vector<int> nums{6,7,8,9};
//     cppprep::leetcode::GenerateAllSubsetsSolution gs;
//     gs.subsets(nums);
//   }
//
//   // {
//   //   std::string digits{"23"};
//   //   cppprep::leetcode::LetterCombinationsOfAPhoneNumberSolution lcs;
//   //   lcs.letterCombinations(digits);
//   // }
//   //
//   // {
//   //   std::vector<std::vector<char>> board{ {'A','B','C','E'},
//   //                            {'S','F','C','S'},
//   //                            {'A','D','E','E'}
//   //                          };
//   //
//   //   std::string word{"ABCCED"};
//   //   cppprep::leetcode::WordSearchSolution ws;
//   //   auto res = ws.exist(board, word);
//   // }
//
//   {
//     std::vector<int> vec{1,2,3};
//     cppprep::leetcode::PermutationsSolution ps;
//     auto res = ps.permute(vec);
//   }
//
//   {
//     Subsets ss;
//     std::vector<int> vec{1,2,3};
//     ss.subsets(vec);
//   }
//
//   {
//     Solution s;
//
//     std::vector<std::vector<int>> vec1{{1,1,1},{1,2,3},{1,2,3}};
//     s.checkValid(vec1);
//
//     std::vector<std::vector<int>> vec2{{1,2,3},{3,1,2},{2,3,1}};
//     s.checkValid(vec2);
//   }
//
//
//   {
//     std::vector<std::string> words{"This", "is", "an", "example", "of", "text", "justification"};
//     textJustification(words);
//   }
//
//   {
//     std::vector<std::string> words{"What","must","be","acknowledgment","shall","be"};
//     textJustification(words);
//   }
//
//   {
//
//     std::vector<std::string> words{"Science","is","what","we","understand","well","enough","to","explain","to","a","computer.","Art","is","everything","else","we","do"};
//     textJustification(words);
//   }
//
//   std::string inp{"hello world"};
//   std::string s(16, ' ');
//   s.append("a");
//   s.copy(inp.data(), inp.length(), 0);
//
//   PackSampleNoPack ps{.ch='A', .n=100, .d=115.5};
//   std::cout << sizeof(PackSample) << std::endl;
//
//   PackSample p;
//   p = *reinterpret_cast<PackSample*>(&ps);
//
//
//   f("test");
//
//   std::cout << "char : " << sizeof(char) << std::endl
//             << "int : " << sizeof(int) << std::endl
//             << "float : " << sizeof(float) << std::endl
//             << "double : " << sizeof(double) << std::endl
//             << "longlong : " << sizeof(long long) << std::endl;
//
//   {
//     cppprep::smartpointers::WalletTest wt;
//     wt.run_test();
//   }
//
//
//   uint32_t n=128, res=0;
//   int bitcount = sizeof(n)*8;
//
//   for (int i = 31; i >= 0; --i) {
//     uint32_t bit = (n & (1u << i)) ? 1 : 0;
//
//     res = (res << 1) | (bit ^ 1);
//
//     // Optional: add space every 4 bits for readability
//     if (i % 4 == 0 && i != 0)
//       std::cout << ' ';
//   }
//   std::cout << "\n";
//
//
//   return 0;
// }

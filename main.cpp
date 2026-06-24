#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "leetcode/fractiontorecurringdecimal.h"
#include "leetcode/medianoftwosortedarrays.h"
#include <vector>
#include <set>
#include <utility>
#include <expected>
#include <sstream>
#include <deque>
#include <iterator>
#include <queue>
#include <chrono>
#include <mdspan>
#include <print>

#include "realworldproblems/OrderBook.h"
#include "leetcode/lrucache.h"
#include "leetcode/dfs/numberofislands.h"
#include "leetcode/slidingwindow/maxslidingwindow.h"
#include "realworldproblems/OrderBookWithMDSpan.h"
#include "smartpointers/smartpointer.h"
#include "realworldproblems/vector_impl.h"
#include "realworldproblems/vector_with_allocator.h"
#include "synchronization/cpu_pinning.h"


using namespace std;

using Price    = int64_t;
using Quantity = int64_t;
using OrderId  = uint64_t;


struct Order {
  OrderId   id;
  bool      isBuy;
  Price     price;
  Quantity  qty;

  friend std::ostream& operator << (std::ostream& os, const Order& order);
};

std::ostream& operator << (std::ostream& os, const Order& order) {
  os << "Order { id : " << order.id
     << ", isBuy : " << order.isBuy
     << ", price : " << order.price
     << ", qty : " << order.qty
     << "}" << std::endl;

  return os;
}

using OrderList = std::list<Order>;
using OrderPtr       = OrderList::iterator;

struct LadderStep
{
  Price             price{0};
  Quantity          totalQty{0};
  OrderList         orders{};

  std::optional<OrderPtr> getOrder(OrderId orderId)
  {
    return std::ranges::find_if(orders, [&orderId](Order& ord) {
      return ord.id == orderId;
    });
  }
  friend std::ostream& operator << (std::ostream& os, const LadderStep& step);
};

std::ostream& operator << (std::ostream& os, const LadderStep& step) {
  os << "Step : " << step.price << ", TotalQty : " << step.totalQty << std::endl;
  std::copy(step.orders.begin(), step.orders.end(),
           std::ostream_iterator<Order>(std::cout));
  std::cout << "\b\b" << std::endl;

  return os;
}

class OrderBook
{
  public:
    using PricingLadder  = std::map<Price, LadderStep>;
    using LadderStepPtr  = PricingLadder::iterator;
    using OrderIdMap     = std::unordered_map<OrderId, std::pair<LadderStepPtr, OrderPtr>>;

    void insert(const Order& order) {
      return (order.isBuy) ? insert(order, _priceLadderBids, _orderIdMapBids):
                             insert(order, _priceLadderAsks, _orderIdMapAsks);
    }

    void modify(const Order& order) {

      return (order.isBuy) ? modify(order, _priceLadderBids, _orderIdMapBids):
                             modify(order, _priceLadderAsks, _orderIdMapAsks);
    }

    void cancel(const Order& order) {

      return (order.isBuy) ? cancel(order, _priceLadderBids, _orderIdMapBids):
                             cancel(order, _priceLadderAsks, _orderIdMapAsks);
    }

    friend std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook);

  private:

    static std::pair<LadderStepPtr, OrderPtr> insertOrUpdatePricingLadder(const Order& order,
                                                                          PricingLadder& pricingLadder) {
      auto [ldrStepPtr, inserted] = pricingLadder.try_emplace(order.price,
                                                                          LadderStep {order.price,
                                                                                      order.qty,
                                                                                OrderList{order}});
      if (!inserted)
      {
        ldrStepPtr->second.orders.emplace_back(order);
        ldrStepPtr->second.price = order.price;
        ldrStepPtr->second.totalQty += order.qty;
      }
      return std::make_pair(ldrStepPtr, std::prev(ldrStepPtr->second.orders.end()));
    }

    void insert(const Order& order, PricingLadder& pricingLadder, OrderIdMap& orderIdMap)
    {
      std::cout << "Inside OrderBook::insert" << std::endl;

      auto ret{ insertOrUpdatePricingLadder(order, pricingLadder)};
      orderIdMap.insert_or_assign(order.id, ret);
    }

    std::pair<LadderStepPtr, OrderPtr> updatePricingLadder(OrderPtr orderPtr, LadderStepPtr ladderStepPtr,
                                                           PricingLadder& pricingLadder)
    {
      auto [newLadderStep, inserted] = pricingLadder.try_emplace(orderPtr->price,
                                                                 LadderStep(orderPtr->price, 0,{}));
      newLadderStep->second.orders.splice(newLadderStep->second.orders.end(),
                                       ladderStepPtr->second.orders,
                                       orderPtr);
      newLadderStep->second.totalQty += orderPtr->qty;

      return std::make_pair(newLadderStep, std::prev(newLadderStep->second.orders.end()));
    }

    static std::optional<LadderStepPtr> extractStepFromPricingLadder(const Order& order, PricingLadder& pricingLadder)
    {
      auto ldrStepPtr{pricingLadder.find(order.price)};
      if (ldrStepPtr == pricingLadder.end())
      {
        return std::nullopt;
      }
      return  ldrStepPtr;
    }


    void modify(const Order& modOrder, PricingLadder& pricingLadder, OrderIdMap& orderIdMap)
    {
      std::cout << "Inside OrderBook::modify" << std::endl;
      const auto orderIdMapItem{orderIdMap.find(modOrder.id)}; // Look for the order in the OrderId map
      if (orderIdMapItem == orderIdMap.end()) {
        std::cout << "Order not found" << std::endl;
        return;
      }

      auto [oldLadderIt, orderPtr] = orderIdMapItem->second;
      bool priceChanged{ orderPtr->price != modOrder.price };
      bool qtyChanged{ orderPtr->qty != modOrder.qty };

      if (!priceChanged && !qtyChanged)
      {
        return;
      }

      if (priceChanged) // OrderPrice has changed. Move to appropriate PriceLevel map
      {
        // Remove old quantity from old price level
        oldLadderIt->second.totalQty -= orderPtr->qty;

        // Update the OrderPtr with the new price
        orderPtr->price = modOrder.price;


        orderIdMapItem->second = {updatePricingLadder(orderPtr, oldLadderIt, pricingLadder)};
      }

      if (qtyChanged)
      {
        auto currLadderStep{orderIdMapItem->second.first};

        currLadderStep->second.totalQty  -= orderPtr->qty;
        currLadderStep->second.totalQty += modOrder.qty;

        orderPtr->qty = modOrder.qty;
      }

      if (oldLadderIt->second.orders.empty())
      {
        pricingLadder.erase(oldLadderIt);
      }

    }

    void cancel(const Order& cancelOrder, PricingLadder& pricingLadder, OrderIdMap& orderIdMap)
    {
      const auto orderIdMapIt{orderIdMap.find(cancelOrder.id)}; // Look for the order in the OrderId map
      if (orderIdMapIt == orderIdMap.end()) {
        std::cout << "Order not found" << std::endl;
        return;
      }

      auto [ladderStepIt, orderIt] = orderIdMapIt->second;
      ladderStepIt->second.totalQty -= orderIt->qty;
      ladderStepIt->second.orders.erase (orderIt);

      orderIdMap.erase(orderIdMapIt);

      if (ladderStepIt->second.orders.empty())
      {
        pricingLadder.erase(ladderStepIt);
      }
    }

    PricingLadder _priceLadderBids{};
    PricingLadder _priceLadderAsks{};

    OrderIdMap _orderIdMapBids{};
    OrderIdMap _orderIdMapAsks{};
};


std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook)
{
  os << "Price ladder Bids"
     << "=================" << std::endl;
  for (auto& [key, val] : orderBook._priceLadderBids)
  {
    os << val << std::endl;
  }

  os << "Price ladder asks"
     << "=================" << std::endl;
  for (auto& [key, val] : orderBook._priceLadderAsks)
  {
    os << val << std::endl;
  }

  return os;
}


struct Message
{
  double d;
  int i;
};

struct foo
{
  int i{};
  double d{};
}_foo;

class Base
{
  public:
    void display(this auto&& self, foo& f) {
      self.displayImpl();
    }

};

class Derived : public Base
{
  public:
    void displayImpl() {
      std::cout << "Inside displayImpl" << std::endl;
    }
};



int main() {

  std::string name{"Alice"};
  std::println(std::cout, "Hello, {}!\n", name);

  {
    cppprep::synchronization::CpuPinning cpuPinning;
    cpuPinning.run_test();
  }

   {
     std::vector<double> data(2 * 5);
     auto book =
       std::mdspan<double, std::extents<size_t, 2, 5>>(data.data(), 2, 5);


     book[0, 0] = 100.05;
     book[0, 1] = 100.04;
     book[0, 2] = 100.03;
     book[0, 3] = 100.02;
     book[0, 4] = 100.01;

     book[1, 0] = 90.05;
     book[1, 1] = 90.04;
     book[1, 2] = 90.03;
     book[1, 3] = 90.02;
     book[1, 4] = 90.01;

     std::cout << "Best Bid : " << book[0,0] << std::endl;
     std::cout << "Best Ask : " << book[1,4] << std::endl;

   }


  {
    cppprep::realworldproblems::OrderBookWithMdSpanTest ob;
    ob.run_test();
  }

  Derived dr;
  dr.display(_foo);


  return 0;
}


// int main() {
//
//   {
//     cppprep::leetcode::MaxSlidingWindowSolutionTest mt;
//     mt.run_test();
//
//   }
//
//   {
//     cppprep::realworldproblems::cache_aligned_allocator<Message> alloc;
//
//     Message* mPtr = alloc.allocate(sizeof(Message));
//   }
//
//   {
//     cppprep::realworldproblems::vector_with_allocator_test vt;
//     vt.run_test();
//   }
//
//   {
//     cppprep::realworldproblems::VectorTest vt;
//     vt.run_test();
//   }
//
//
//   {
//     cppprep::realworldproblems::OrderBookHFT_Test test{};
//     test.run_test();
//   }
//
//   {
//     OrderBook ob;
//
//     // std::expected<int, int> e = std::unexpected(42);
//     // std::cout << "std::expected works!\n";
//
//     std::vector<Order> orders{
//       { 111, true, 222, 333},
//       { 112, true, 223, 334},
//       { 113, false, 224, 335},
//       { 114, true, 225, 336},
//       { 115, false, 226, 337},
//     };
//
//
//     for (auto& order  : orders)
//     {
//       ob.insert(order);
//     }
//
//     std::cout << ob << std::endl;
//
//     // Modification test
//     {
//       Order modOrder1{orders[4]};
//       modOrder1.price = 22600;
//       modOrder1.qty = 3377;
//
//       ob.modify(modOrder1);
//       std::cout << ob << std::endl;
//     }
//
//     {
//       Order insertOrder{};
//       insertOrder.id = 22600;
//       insertOrder.price = 22600;
//       insertOrder.qty = 2000;
//       insertOrder.isBuy = false;
//
//       ob.insert(insertOrder);
//       std::cout << ob << std::endl;
//     }
//
//        Order cancOrder1{115,false};
//     ob.cancel(cancOrder1);
//     std::cout << ob << std::endl;
//   }
//
//
//   return 0;
// }



// int main() {
//   Solution sol;
//   {
//     string s1{"babad"};
//     sol.longestPalindrome(s1);
//
//     string s2{"cbbd"};
//     sol.longestPalindrome(s2);
//   }
//
//
//   {
//     LRUCacheSolutionTest test;
//     test.run_test();
//   }
//
//   {
//     cppprep::smartpointers::SharedPtrTest sol;
//     sol.run_test();
//   }
//
//   return 0;
// }


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

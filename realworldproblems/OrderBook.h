#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <deque>

namespace cppprep::realworldproblems
{

  enum class OrderType : int32_t
  {
    MARKET = 0,
    LIMIT = 1
  };

  enum class Side : int32_t
  {
    BUY = 0,
    SELL = 1,
  };

  struct alignas(std::max_align_t) Order
  {
    int64_t orderId{0};
    int64_t price{};
    int64_t quantity{};
    int32_t side{0};
    int32_t orderType{0};
  };

  struct PriceLevel
  {
    int64_t           totalQuantity{0};
    std::deque<Order> orders{};
  };

  class OrderBook
  {
    private:
      using PriceLevelMap = std::pmr::unordered_map<int64_t, PriceLevel>;

      alignas(64) PriceLevelMap _priceLevels[2]{};

      int64_t _bestBidPrice{0};
      int64_t _bestAskPrice{0};

      std::unordered_map<int64_t, std::pair<int64_t, char>> orderLocation;

    public:


      void addOrder(uint64_t orderId, int64_t price, int64_t quantity, Side side) {
        if (quantity <= 0.0) return;

        auto& book{_priceLevels[static_cast<int>(side)]};
        auto& level{book[price]};
        level.orders.emplace_back(orderId, quantity);
        level.totalQuantity += quantity;

        // Update the best prices
        if (side == Side::BUY) {
          _bestBidPrice = std::max(_bestBidPrice, price);
        } else if(side == Side::SELL) {
          _bestAskPrice = std::min(_bestAskPrice, price);
        }
      }
  };


  class OrderBookTest
  {
    public:
  };

}

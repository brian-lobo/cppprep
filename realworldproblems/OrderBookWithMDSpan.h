#pragma once

#include <mdspan>
#include <array>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <optional>
#include <vector>
#include <cstdint>
#include <iostream>


namespace cppprep::realworldproblems
{
    enum class Side { Bid = 0, Ask = 1};

    struct Order
    {
      double   px;
      uint64_t qty;
      bool     isBuy;
    };

    class OrderBookWithMdSpan
    {

      private:
        static constexpr size_t MAX_DEPTH{5};
        using PX_DEPTH = std::array<double, MAX_DEPTH>;
        using QTY_DEPTH = std::array<double, MAX_DEPTH>;

        // BIDS : sorted descending by price
        PX_DEPTH    _bidPrices{};
        QTY_DEPTH   _bidQtys{};
        size_t      _bidCount{0};

        // ASKS : sorted descending by price
        PX_DEPTH    _askPrices{};
        QTY_DEPTH   _askQtys{};
        size_t      _askCount{0};

        // === mdspan Views (zero-cost, non-owning) ===
        auto bidPrices() const
        {
          return std::mdspan<const double,
                             std::extents<size_t, std::dynamic_extent>> (_bidPrices.data(), _bidCount);
        }
        auto bidQtys() const
        {
          return std::mdspan<const double,
                             std::extents<size_t, std::dynamic_extent>> (_bidQtys.data(), _bidCount);
        }

        auto askPrices() const
        {
          return std::mdspan<const double,
                             std::extents<size_t, std::dynamic_extent>> (_askPrices.data(), _askCount);
        }
        auto askQtys() const
        {
          return std::mdspan<const double,
                             std::extents<size_t, std::dynamic_extent>> (_bidQtys.data(), _bidCount);
        }

        std::optional<double> bestBid() const {
          return _bidCount >0 ? std::optional<double>(_bidPrices[0]) : nullopt;
        }

        std::optional<double> bestAsk() const {
          return _askCount >0 ? std::optional<double>(_askPrices[0]) : nullopt;
        }

        std::optional<double> midPrice() const {
          auto bb{bestBid()};
          auto ba{bestAsk()};

          return (bb && ba) ? std::optional<double>(((*bb + *ba)/2.0)) : std::nullopt;
        }

        std::optional<double> spread() {
          auto bb{bestBid()};
          auto ba{bestAsk()};

          return (bb && ba) ? std::optional<double>(((*bb - *ba))) : std::nullopt;
        }

        template<bool IsBuy>
        constexpr auto getComparator()
        {
          if constexpr(IsBuy) {// Buy comparator
            return [](double a, double b) { return a > b; };
          }
          else { // Sell comparator
            return [](double a, double b) { return a < b; };
          }
        }

        template <bool IsBuy>
        void update(const Order& order) {

            static const auto cmp = getComparator<IsBuy>();

            PX_DEPTH& prices = IsBuy ? _bidPrices : _askPrices;
            QTY_DEPTH& qtys = IsBuy ? _bidQtys : _askQtys;
            size_t& count  = IsBuy ? _bidCount : _askCount;

            auto it = std::lower_bound(prices.begin(), prices.begin() + count,
                                       order.px, cmp);

            size_t idx = it - prices.begin();
            if (idx < count && prices[idx] == order.px)
            {
              if (order.qty == 0) {
                std::move(prices.begin() + idx + 1,
                          prices.begin() + count,
                          prices.begin() + idx);
                std::move(qtys.begin() + idx + 1,
                          qtys.begin() + count,
                          qtys.begin() + idx);
                count--;
              }
              else
              {
                qtys[idx] = order.qty;
              }
            }
            else
            {
              if (order.qty == 0 || count >= MAX_DEPTH) return;

              std::move_backward(prices.begin() + idx,
                                 prices.begin() + count,
                                 prices.begin() + count + 1);
              std::move_backward(qtys.begin() + idx,
                                 qtys.begin() + count,
                                 qtys.begin() + count + 1);
              prices[idx] = order.px;
              qtys[idx] = order.qty;
              ++count;
            }
          }

      public:

        void print(size_t levels = 10) const {
          std::cout << std::fixed << std::setprecision(2);
          std::cout << "\n=== Order Book (Top " << levels << ") ===\n";
          std::cout << "     Bids                  Asks\n";
          std::cout << "Price     Qty     | Price     Qty\n";
          std::cout << "-------------------|-------------------\n";

          size_t max_show = std::min(std::max(_bidCount, _askCount), levels);

          for (size_t i = 0; i < max_show; ++i) {
            if (i < _bidCount) {
              std::cout << std::setw(8) << _bidPrices[i]
                        << " " << std::setw(8) << _bidQtys[i];
            } else {
              std::cout << "                  ";
            }
            std::cout << " | ";
            if (i < _askCount) {
              std::cout << std::setw(8) << _askPrices[i]
                        << " " << std::setw(8) << _askQtys[i];
            }
            std::cout << "\n";
          }
          std::cout << "=====================================\n";
        }

        void update(const Order& order) {
          if (order.isBuy) {
            update<true>(order);
          }
          else {
            update<false>(order);
          }
        }
    };


  class OrderBookWithMdSpanTest
  {
    public:
      void run_test() {
        OrderBookWithMdSpan ob;

        ob.update(Order{100.05, 1200, true});
        ob.update(Order{100.04, 800,  true});
        ob.update(Order{100.06, 900, true});
        ob.update(Order{90.75, 600,  true});
        ob.update(Order{90.99, 400, true});
        ob.update(Order{100.04, 500,  true});
        ob.update(Order{100.05, 1200, true});
        ob.update(Order{100.04, 800,  true});

        ob.update(Order{100.04, 1500, false});

      }


  };


}

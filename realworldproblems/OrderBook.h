#pragma once

#include <ranges>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <optional>
#include <vector>
#include <map>
#include <unordered_map>


namespace cppprep::realworldproblems
{

  struct TimestampGenerator
  {
    private:
      template<typename Clock, typename Dur, typename TargetDur=std::chrono::milliseconds>
      uint64_t chrono_to_uint64(const std::chrono::time_point<Clock,Dur>& tp) {
        auto target = std::chrono::duration_cast<TargetDur>(tp.time_since_epoch());
        return static_cast<uint64_t>(target.count());
      }

    public:
      uint64_t operator()() {
        auto high_res{std::chrono::high_resolution_clock::now()};
        uint64_t val{chrono_to_uint64(high_res) };

        return val;
      }
  };

    using Price     = int64_t;
    using Quantity  = int64_t;
    using OrderId   = uint64_t;
    using Timestamp = uint64_t;

    struct OrderHFT {
      OrderId   id{};
      bool      isBuy{};
      Price     px{};
      Quantity  qty{};
      Timestamp timestamp{TimestampGenerator{}()};

      friend std::ostream& operator << (std::ostream& os, const OrderHFT& order);
    };

    class OrderBookHFT
    {
      private:
        std::vector<OrderHFT> orders;
        std::unordered_map<OrderId, size_t> orderIdMap;

        struct PriceLevel
        {
          std::vector<size_t> orderIndices;
          int64_t totalQuantity{0};
        };

        using PriceLevelMap = std::map<Price, PriceLevel>;

        std::map<Price, PriceLevel> bids;
        std::map<Price, PriceLevel> asks;

      public:
        friend std::ostream& operator << (std::ostream& os, const OrderBookHFT& orderBook);
        friend std::ostream& operator << (std::ostream& os, const PriceLevel& priceLevel);

        void insert(OrderHFT order) {
          size_t idx{orders.size()};
          orders.push_back(order);
          orderIdMap.emplace(order.id, idx);

          auto& priceLevelMap {order.isBuy ? bids : asks};
          auto& level {priceLevelMap[order.px]};

          level.orderIndices.push_back(idx);
          level.totalQuantity += order.qty;
        }


        // void remove_from_price_level(Side side, double price, size_t order_idx) {
        void removeFromPriceLevel(const OrderHFT& order, size_t orderIdx) {
          auto priceLevel{getPriceLevelEntry(order.isBuy, order.px)};
          if (priceLevel.has_value())
          {
            auto& orderIndices{priceLevel.value()->second.orderIndices};
            std::erase (orderIndices, orderIdx);
            priceLevel.value()->second.totalQuantity -= order.qty;

            if (orderIndices.empty())
            {
              auto& priceLevelMap = order.isBuy ? bids : asks;
              priceLevelMap.erase(priceLevel.value());
            }
          }
        }

        void updatePriceLevelIndex(const OrderHFT& order, size_t oldIdx, size_t newIdx)
        {
          auto priceLevel{getPriceLevelEntry(order.px, order.isBuy)};
          if (priceLevel.has_value())
          {
            auto& indices = priceLevel.value()->second.orderIndices;
            // Replace oldIdx with newIdx
            for (size_t& i : indices) {
              if (indices[i] == oldIdx) {
                indices[i] = newIdx;
                return;   // assuming indices are unique
              }
            }
          }
        }

        void cancel(OrderId orderId) {
          // Find and remove from the orderIdMap
          auto orderIdItr{orderIdMap.find(orderId)};
          if (orderIdItr == orderIdMap.end()) {
            return;
          }

          size_t idx = orderIdItr->second;
          const OrderHFT& ord = orders[idx];

          removeFromPriceLevel(ord, idx);
          size_t lastIdx{orders.size() -1};
          if (idx != lastIdx)
          {
            OrderHFT& lastOrder{orders.back()};

            // Update the map for the order that was moved.
            orderIdMap[lastOrder.id] = idx;

            std::swap(orders[idx], orders[lastIdx]);

            // === CRITICAL: Update PriceLevel for the moved order ===
            // updatePriceLevelIndex(lastOrder, lastIdx, idx);
            updatePriceLevelIndex(orders[idx], lastIdx, idx);
          }
          orders.pop_back();
          orderIdMap.erase(orderIdItr);
        }

        void modify(OrderHFT modOrder) {
          auto orderIdItr{orderIdMap.find(modOrder.id)};
          if (orderIdItr == orderIdMap.end()) {
            return;
          }

          OrderHFT order{ orders[orderIdItr->second]};
          Quantity currQty{order.qty};

          bool isPriceModified{modOrder.px != order.px};
          bool isQtyModified{modOrder.qty != order.qty};

          if (isPriceModified)
          {
            removeFromPriceLevel(order, orderIdItr->second);
            auto& currOrderHFT{orders[orderIdItr->second]};
            currOrderHFT.px = modOrder.px;

            auto& priceLevelMap = order.isBuy ? bids : asks;
            auto& priceLevel{ priceLevelMap[modOrder.px]};
            priceLevel.orderIndices.emplace_back(orderIdItr->second);
            priceLevel.totalQuantity += modOrder.qty;
          }

          if (isQtyModified)
          {
            auto& currOrderHFT{orders[orderIdItr->second]};
            currOrderHFT.qty = modOrder.qty;

            if (!isPriceModified)
            {
              auto priceLevel{getPriceLevelEntry(order.isBuy, order.px)};
              if (priceLevel.has_value())
              {
                priceLevel.value()->second.totalQuantity -= currQty;
                priceLevel.value()->second.totalQuantity += modOrder.qty;
              }
            }
          }
        }

        std::optional<PriceLevelMap::iterator> getPriceLevelEntry(Price px, bool isBuy) {
          auto& priceLevelMap = isBuy ? bids : asks;
          auto priceLevelItr{priceLevelMap.find(px)};
          if (priceLevelItr != priceLevelMap.end()){
            return priceLevelItr;
          }
          return std::nullopt;
        }
    };

    std::ostream& operator << (std::ostream& os, const OrderHFT& order);
    std::ostream& operator << (std::ostream& os, const OrderBookHFT& orderBook);
    std::ostream& operator << (std::ostream& os, const OrderBookHFT::PriceLevel& priceLevel);

    class  OrderBookHFT_Test
    {
      public:
        void run_test() {
          OrderBookHFT ob;

          std::vector<OrderHFT> orders{
            OrderHFT{.id=111, .isBuy= true, .px=1, .qty=11},
            OrderHFT{.id=112, .isBuy=true,  .px=1, .qty=12},
            OrderHFT{.id=222, .isBuy=true,  .px=2, .qty=22},
            OrderHFT{.id=333, .isBuy=false, .px=3, .qty=33},
            OrderHFT{.id=444, .isBuy=true,  .px=4, .qty=44},
            OrderHFT{.id=555, .isBuy=false, .px=5, .qty=55}
          };

          // Insert the first set of orders
          std::ranges::for_each(orders, [&ob](OrderHFT order) {
            ob.insert(order);
          });

          ob.cancel(111);

          OrderHFT modOrder{112, true, 2, 13};
          ob.modify(modOrder);

        }
    };

}

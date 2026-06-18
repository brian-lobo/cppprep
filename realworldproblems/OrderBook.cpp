#include <iostream>
#include "OrderBook.h"

namespace cppprep::realworldproblems
{
  std::ostream& operator << (std::ostream& os, const OrderHFT& order) {
    os << "OrderHFT{"
       << "id: " << order.id
       << ", isBuy: " << std::boolalpha << order.isBuy
       << ", px: " << order.px
       << ", qty: " << order.qty
       << ", timestamp : " << order.timestamp
       << "}" << std::endl;

    return os;
  }

  std::ostream& operator << (std::ostream& os, const OrderBookHFT& orderBook)
  {
    std::for_each(orderBook.orders.cbegin(), orderBook.orders.cend(), [&os, &orderBook](const OrderHFT& order) {
      os <<  order;
    });

    return os;
  }

  std::ostream& operator << (std::ostream& os, const OrderBookHFT::PriceLevel& priceLevel)
  {
    os << "PriceLevel" << std::endl
       << "==========" << std::endl
       << "Total Quantity : " << priceLevel.totalQuantity  << std::endl
       << "Order Indices  : ";

    std::copy(priceLevel.orderIndices.begin(),
              priceLevel.orderIndices.end(),
              std::ostream_iterator<size_t>(std::cout, ","));
    std::cout << "\b\b";

    return os;
  }
}
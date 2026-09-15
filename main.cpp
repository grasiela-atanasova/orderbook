#include <iostream>

#include "order.hpp"
#include "orderbook.hpp"

int main()
{
    OrderBook book;
    book.add_limit_order({1, Side::Buy,  OrderType::Limit, 100, 40, 1});
    book.add_limit_order({2, Side::Buy,  OrderType::Limit,  99, 20, 2});
    book.add_limit_order({3, Side::Sell, OrderType::Limit, 101, 50, 3});
    book.add_limit_order({4, Side::Sell, OrderType::Limit, 102, 30, 4});
    book.add_limit_order({5, Side::Buy,  OrderType::Limit, 100, 15, 5}); // same price as #1
    book.print_book();
    return 0;
}
#include <iostream>

#include "order.hpp"
#include "orderbook.hpp"

int main()
{
    OrderBook book;

    ///TESTING add_limit_order
    
    /*book.add_limit_order({1, Side::Buy,  OrderType::Limit, 100, 40, 1});
    book.add_limit_order({2, Side::Buy,  OrderType::Limit,  99, 20, 2});
    book.add_limit_order({3, Side::Sell, OrderType::Limit, 101, 50, 3});
    book.add_limit_order({4, Side::Sell, OrderType::Limit, 102, 30, 4});
    book.add_limit_order({5, Side::Buy,  OrderType::Limit, 100, 15, 5}); // same price as #1
    book.print_book();*/

    ///TESTING add_order

    book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 102, 30, 2});
    book.print_book();

    std::cout << "--- Test A: incoming buy 30 (smaller than resting 50) ---\n";
    auto t1 = book.add_order({3, Side::Buy, OrderType::Limit, 101, 30, 3});
    for(auto& t : t1) std::cout << " trade " << t.quantity << " @ " << t.price << "\n";
    book.print_book();

    std::cout << "-- test B: incoming buy 100 (sweeps both levels) --\n";
    auto t2 = book.add_order({4, Side::Buy, OrderType::Limit, 103, 100, 4});
    for (auto& t : t2) std::cout << " trade " << t.quantity << " @ " << t.price << "\n";
    book.print_book();
    
    return 0;
}
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

    /*book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 102, 30, 2});
    book.print_book();

    std::cout << "--- Test A: incoming buy 30 (smaller than resting 50) ---\n";
    auto t1 = book.add_order({3, Side::Buy, OrderType::Limit, 101, 30, 3});
    for(auto& t : t1) std::cout << " trade " << t.quantity << " @ " << t.price << "\n";
    book.print_book();

    std::cout << "-- test B: incoming buy 100 (sweeps both levels) --\n";
    auto t2 = book.add_order({4, Side::Buy, OrderType::Limit, 103, 100, 4});
    for (auto& t : t2) std::cout << " trade " << t.quantity << " @ " << t.price << "\n";
    book.print_book();*/

    ///TESTING cancel

    book.add_order({1, Side::Buy, OrderType::Limit, 100, 40, 1});
    book.add_order({2, Side::Buy, OrderType::Limit, 100, 20, 2});
    book.print_book();

    std::cout << "cancel(1) = " << book.cancel(1) << "\n";   // expect 1
    book.print_book();                                       // expect 100 | qty 20 (only #2)
    std::cout << "cancel(2) = " << book.cancel(2) << "\n";   // expect 1
    book.print_book();                                       // expect 100 level gone
    std::cout << "cancel(99) = " << book.cancel(99) << "\n"; // expect 0

    // sync test: fully fill an order, then try to cancel it
    book.add_order({10, Side::Sell, OrderType::Limit, 105, 30, 10});
    book.add_order({11, Side::Buy,  OrderType::Limit, 105, 30, 11}); // fills #10
    book.print_book();

    std::cout << "cancel(10) = " << book.cancel(10) << "\n"; // expect 0, NO crash
    book.print_book();
    
    return 0;
}
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

    /*book.add_order({1, Side::Buy, OrderType::Limit, 100, 40, 1});
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
    */

    /// TESTING OrderType::Market
    /*book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 102, 30, 2});
    book.print_book();

    std::cout << "--- market buy 60 ---\n";
    auto t = book.add_order({3, Side::Buy, OrderType::Market, 0, 60, 3});
    for(auto& x: t) std::cout << "traded " << x.quantity << " @ " << x.price << "\n";
    book.print_book();
    
    //Testing the discarding
    // --- Test: market order bigger than the whole book (mass discard) ---
    std::cout << "--- market buy 500 (only 20 left) ---\n";
    auto t2 = book.add_order({4, Side::Buy, OrderType::Market, 0, 500, 4});
    for (auto& x : t2) std::cout << "  trade " << x.quantity << " @ " << x.price << "\n";
    book.print_book();
    // expect: takes remaining 20 @ 102; asks now EMPTY; ~480 discarded (see the note); NO bid rests; no crash

    // --- Test: market order into an EMPTY book (nothing to fill) ---
    std::cout << "--- market buy into empty book ---\n";
    auto t3 = book.add_order({5, Side::Buy, OrderType::Market, 0, 50, 5});
    for (auto& x : t3) std::cout << "  trade " << x.quantity << " @ " << x.price << "\n";
    book.print_book();
    // expect: ZERO trades; whole 50 discarded; book unchanged; no crash
    */

    ///TESTING OrderType::Ioc
    // IOC: fills what it can at its price cap, discards the rest
    book.add_order({10, Side::Sell, OrderType::Limit, 105, 50, 10});
    book.print_book();
    
    std::cout << "--- IOC buy 80 @ 105 ---\n";
    auto i = book.add_order({11, Side::Buy, OrderType::Ioc, 105, 80, 11});
    for (auto& x : i) std::cout << "  trade " << x.quantity << " @ " << x.price << "\n";
    // expect: 50 @ 105 filled; 30 discarded (note prints); NO bid rests at 105

    // IOC that can't cross at all → zero trades, whole thing discarded
    book.add_order({12, Side::Sell, OrderType::Limit, 110, 40, 12});
    std::cout << "--- IOC buy 40 @ 108 (below best ask 110) ---\n";
    auto i2 = book.add_order({13, Side::Buy, OrderType::Ioc, 108, 40, 13});
    for (auto& x : i2) std::cout << "  trade " << x.quantity << " @ " << x.price << "\n";
    book.print_book();
    // expect: ZERO trades (108 < 110, doesn't cross); all 40 discarded; nothing rests 

    return 0;
}
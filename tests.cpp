#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "orderbook.hpp"

TEST_CASE("full fill: equal quantities produce one trade and empty the level") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    auto trades = book.add_order({2, Side::Buy, OrderType::Limit, 101, 50, 2});
    CHECK(trades.size() == 1);
    CHECK(trades[0].quantity == 50);
    CHECK(trades[0].price == 101);
    CHECK(book.quantity_at(Side::Sell, 101) == 0);   // ask fully consumed
}

TEST_CASE("partial fill: resting order stays with reduced quantity") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    auto trades = book.add_order({2, Side::Buy, OrderType::Limit, 101, 30, 2});
    CHECK(trades.size() == 1);
    CHECK(trades[0].quantity == 30);
    CHECK(book.quantity_at(Side::Sell, 101) == 20);  // 20 still resting
}

TEST_CASE("multi-level sweep fills across levels for the correct total") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 102, 30, 2});
    auto trades = book.add_order({3, Side::Buy, OrderType::Limit, 103, 80, 3});
    CHECK(trades.size() == 2);                         // one per level
    uint64_t total = trades[0].quantity + trades[1].quantity;
    CHECK(total == 80);                                // catches the remaining/incoming.quantity bug
    CHECK(book.quantity_at(Side::Sell, 101) == 0);
    CHECK(book.quantity_at(Side::Sell, 102) == 0);
}

TEST_CASE("non-crossing limit order rests, no trades") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 105, 40, 1});
    auto trades = book.add_order({2, Side::Buy, OrderType::Limit, 100, 40, 2}); // 100 < 105
    CHECK(trades.empty());
    CHECK(book.quantity_at(Side::Buy, 100) == 40);    // rested as a bid
}

TEST_CASE("time priority: earlier order at same price fills first") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 100, 30, 1}); // earlier
    book.add_order({2, Side::Sell, OrderType::Limit, 100, 30, 2}); // later
    auto trades = book.add_order({3, Side::Buy, OrderType::Limit, 100, 30, 3});
    CHECK(trades.size() == 1);
    CHECK(trades[0].sell_order_id == 1);              // #1 (earlier) matched, not #2
    CHECK(book.quantity_at(Side::Sell, 100) == 30);   // #2 still there
}

TEST_CASE("cancel removes a resting order") {
    OrderBook book;
    book.add_order({1, Side::Buy, OrderType::Limit, 100, 40, 1});
    CHECK(book.cancel(1) == true);
    CHECK(book.quantity_at(Side::Buy, 100) == 0);
}

TEST_CASE("cancel of a filled order returns false and does not crash") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 100, 30, 1});
    book.add_order({2, Side::Buy,  OrderType::Limit, 100, 30, 2}); // fully fills #1
    CHECK(book.cancel(1) == false);                   // #1 is gone; index stayed in sync
}

TEST_CASE("cancel of a never-existing id returns false") {
    OrderBook book;
    CHECK(book.cancel(999) == false);
}

TEST_CASE("market order matches regardless of price and does not rest") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 101, 50, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 102, 30, 2});
    auto trades = book.add_order({3, Side::Buy, OrderType::Market, 0, 60, 3});
    CHECK(trades.size() == 2);
    CHECK(book.quantity_at(Side::Sell, 102) == 20);   // took 50@101 + 10@102
    CHECK(book.quantity_at(Side::Buy, 0) == 0);        // nothing rested
}

TEST_CASE("IOC fills at its cap and discards the remainder") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 105, 50, 1});
    auto trades = book.add_order({2, Side::Buy, OrderType::Ioc, 105, 80, 2});
    CHECK(trades.size() == 1);
    CHECK(trades[0].quantity == 50);
    CHECK(book.quantity_at(Side::Buy, 105) == 0);      // 30 discarded, not rested
}

TEST_CASE("FOK with insufficient liquidity does nothing") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 110, 40, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 111, 40, 2}); // 80 available
    auto trades = book.add_order({3, Side::Buy, OrderType::Fok, 111, 100, 3}); // wants 100
    CHECK(trades.empty());
    CHECK(book.quantity_at(Side::Sell, 110) == 40);    // book UNTOUCHED
    CHECK(book.quantity_at(Side::Sell, 111) == 40);
}

TEST_CASE("FOK with exactly enough liquidity fully fills") {
    OrderBook book;
    book.add_order({1, Side::Sell, OrderType::Limit, 110, 40, 1});
    book.add_order({2, Side::Sell, OrderType::Limit, 111, 40, 2});
    auto trades = book.add_order({3, Side::Buy, OrderType::Fok, 111, 80, 3});
    CHECK(trades.size() == 2);
    CHECK(book.quantity_at(Side::Sell, 110) == 0);
    CHECK(book.quantity_at(Side::Sell, 111) == 0);
}
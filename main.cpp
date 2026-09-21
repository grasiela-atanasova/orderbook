#include <iostream>
#include <random>

#include "order.hpp"
#include "orderbook.hpp"

int main()
{
    OrderBook book;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> side_dist (0, 1);
    std::uniform_int_distribution<uint64_t> price_dist (95, 105);
    std::uniform_int_distribution<uint16_t> qty_dist (1, 50);
    std::uniform_int_distribution<uint16_t> type_dist (1, 100);

    const int N = 10000;
    uint64_t total_trades = 0;

    for(uint64_t i = 1; i <= N; i ++)
    {
        Side side = (side_dist(rng) == 0) ? Side::Buy : Side::Sell;
        OrderType type;
        if(type_dist(rng) <= 80)      type = OrderType::Limit;
        else if(type_dist(rng) <= 90) type = OrderType::Market;
        else if(type_dist(rng) <= 97) type = OrderType::Ioc;
        else                          type = OrderType::Fok;

        uint64_t price = price_dist (rng);
        uint64_t qty = qty_dist (rng);
        Order o = {i, side, type, price, qty, i};
        auto trades = book.add_order(o);
        total_trades += trades.size();
    }

    std::cout << "Processed " << N << " orders, executed " << total_trades << " trades\n";

    return 0;
}
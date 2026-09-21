#include <iostream>
#include <random>
#include <chrono>

#include "orderbook.hpp"

int main()
{
    OrderBook book;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> side_dist (0, 1);
    std::uniform_int_distribution<uint64_t> price_dist (95, 105);
    std::uniform_int_distribution<uint16_t> qty_dist (1, 50);
    std::uniform_int_distribution<uint16_t> type_dist (1, 100);

    const int N = 1000000;
    uint64_t total_trades = 0;

    std::vector <Order> orders;
    orders.reserve(N);

    for(uint64_t i = 1; i <= N; i ++)
    {
        Side side = (side_dist(rng) == 0) ? Side::Buy : Side::Sell;
        auto type_num = type_dist(rng);
        OrderType type;
        if(type_num <= 80)      type = OrderType::Limit;
        else if(type_num <= 90) type = OrderType::Market;
        else if(type_num <= 97) type = OrderType::Ioc;
        else                          type = OrderType::Fok;

        Order o = {i, side, type, price_dist(rng), qty_dist(rng), i};
        orders.push_back(o);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for(const Order& o: orders)
    {
        auto trades = book.add_order(o);
        total_trades += trades.size();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    book.print_book();
    std::cout << "Processed " << N << " orders in " << elapsed.count() << " s\n";
    std::cout << "Trades: " << total_trades << "\n";
    std::cout << "Throughput: " << (N/elapsed.count()) << " orders/sec\n";
    return 0;
}
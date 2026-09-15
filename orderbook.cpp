#include <iostream>
#include "orderbook.hpp"
#include <iomanip>

void OrderBook::add_limit_order(const Order& order){
    if(order.side == Side::Buy)
    {
        bids_[order.price].push_back(order);
    }
    else
    {
        asks_[order.price].push_back(order);
    }
}

void OrderBook::print_book() const{
    std::cout << "-------- ORDER BOOK ---------\n";
    std::cout << "ASKS (sell):\n";
    for(auto it = asks_.rbegin(); it != asks_.rend(); ++it)
    {
        uint64_t total = 0;
        for(const Order& o: it->second) total += o.quantity;
        std::cout << " " << std::setw(5) << it->first << " | qty " << std::setw(3) << total << "\n";
    }
    std::cout << "-------- spread ---------\n";
    std::cout << "BIDS (buy):\n";
    for(const auto& [price, level]: bids_)
    {
        uint64_t total = 0;
        for(const Order& o: level) total += o.quantity;
        std::cout << " " << std::setw(5) << price << " | qty " << std::setw(3) << total << "\n";
    }
}
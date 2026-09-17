#include <iostream>
#include "orderbook.hpp"
#include <iomanip>
#include <algorithm> 

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

std::vector<Trade> OrderBook::add_order(const Order& incoming)
{
    std::vector<Trade> trades;
    uint64_t remaining = incoming.quantity;
    if(incoming.side == Side::Buy)
    {
        while(remaining > 0 && !asks_.empty() && asks_.begin()->first <= incoming.price)
        {
            auto best = asks_.begin();
            int64_t trade_price = best -> first;
            Order& resting = best -> second.front();
            uint64_t traded = std::min(resting.quantity, incoming.quantity);
            trades.push_back({incoming.id, resting.id, trade_price, traded});
            remaining -= traded;
            resting.quantity -= traded;

            if(resting.quantity == 0) best -> second.pop_front();
            if(best->second.empty())  asks_.erase(best);
        }
    }
    else if(incoming.side == Side::Sell)
    {
        while(remaining > 0 && !bids_.empty() && bids_.begin()->first >= incoming.price)
        {
            auto best = bids_.begin();
            Order& resting = best->second.front();
            int64_t trade_price = resting.price;
            uint64_t traded = std::min(incoming.quantity, resting.quantity);
            remaining -= traded;
            resting.quantity -= traded;
            trades.push_back({resting.id, incoming.id, trade_price, traded});
            if(resting.quantity == 0) best->second.pop_front();
            if(best->second.empty()) bids_.erase(best);
        }
    }
    if(remaining > 0)
    {
        Order leftover = incoming;
        leftover.quantity = remaining;
        if(incoming.side == Side::Buy) bids_[leftover.price].push_back(leftover);
        else if(incoming.side == Side::Sell) asks_[leftover.price].push_back(leftover);
    }

    return trades;
}
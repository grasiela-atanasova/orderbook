#include <iostream>
#include "orderbook.hpp"
#include <iomanip>
#include <algorithm> 

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

bool OrderBook::can_fully_fill(const Order& incoming) const
{
    uint64_t available = 0;
    if(incoming.side == Side::Buy)
    {
        for(auto [price, level]: asks_)
        {
            if(price > incoming.price) break;
            for(const Order& o : level)
            {
                available += o.quantity;
                if(available >= incoming.quantity) return true;
            }
        }
    }
    else if(incoming.side == Side::Sell)
    {
        for(auto [price, level]: bids_)
        {
            if(price < incoming.price) break;
            for(const Order& o: level)
            {
                available += o.quantity;
                if(available >= incoming.quantity) return true;
            }
        }
    }
    
    return available >= incoming.quantity;
}
std::vector<Trade> OrderBook::add_order(const Order& incoming)
{
    std::vector<Trade> trades;
    uint64_t remaining = incoming.quantity;
    if(incoming.type == OrderType::Fok && can_fully_fill(incoming) == false) 
    {
        std::cout << "[Fok order #" << incoming.id << " killed - not enough liquidity to fill the order of quantity " << incoming.quantity << "]\n";
        return {};
    }

    if(incoming.side == Side::Buy)
    {
        while(remaining > 0 && !asks_.empty() && (asks_.begin()->first <= incoming.price || incoming.type == OrderType::Market))
        {
            auto best = asks_.begin();
            int64_t trade_price = best -> first;
            Order& resting = best -> second.front();
            uint64_t traded = std::min(resting.quantity, remaining);
            trades.push_back({incoming.id, resting.id, trade_price, traded});
            remaining -= traded;
            resting.quantity -= traded;

            if(resting.quantity == 0) 
            {
                index_.erase(resting.id);
                best -> second.pop_front();
            }
            if(best->second.empty())  asks_.erase(best);
        }
    }
    else if(incoming.side == Side::Sell)
    {
        while(remaining > 0 && !bids_.empty() && (bids_.begin()->first >= incoming.price || incoming.type == OrderType::Market))
        {
            auto best = bids_.begin();
            Order& resting = best->second.front();
            int64_t trade_price = resting.price;
            uint64_t traded = std::min(remaining, resting.quantity);
            remaining -= traded;
            resting.quantity -= traded;
            trades.push_back({resting.id, incoming.id, trade_price, traded});
            if(resting.quantity == 0) 
            {
                index_.erase(resting.id);
                best->second.pop_front();
            }
            if(best->second.empty()) bids_.erase(best);
        }
    }
    if(remaining > 0 && incoming.type == OrderType::Limit)
    {
        Order leftover = incoming;
        leftover.quantity = remaining;
        if(incoming.side == Side::Buy) 
        {
            PriceLevel& level = bids_[leftover.price];
            level.push_back(leftover);
            index_[leftover.id] = {leftover.side, leftover.price, std::prev(level.end())};

        }
        else if(incoming.side == Side::Sell)
        {
            PriceLevel& level = asks_[leftover.price];
            level.push_back(leftover);
            index_[leftover.id] = {leftover.side, leftover.price, std::prev(level.end())};
        }
    }

    else if(remaining > 0 && incoming.type == OrderType::Market)
    {
        std::cout << "discarded[" << remaining << " unfilled units of order #" << incoming.id << "], OrderType::Market\n";
    }

    else if(remaining > 0 && incoming.type == OrderType::Ioc)
    {
        std::cout << "discarded[" << remaining << " unfilled units of order #" << incoming.id << "], OrderType::Market\n";
    }
                                                            
    return trades;
}

bool OrderBook::cancel(uint64_t order_id)
{
    auto found = index_.find(order_id);
    if(found == index_.end()) return false;
    OrderLocation loc = found->second;

    if(loc.side == Side::Buy)
    {
        PriceLevel& level = bids_[loc.price];
        level.erase(loc.it);
        if(level.empty()) bids_.erase(loc.price);
    }
    else if(loc.side == Side::Sell)
    {
        PriceLevel& level = asks_[loc.price];
        level.erase(loc.it);
        if(level.empty()) asks_.erase(loc.price);
    }
    
    index_.erase(found);
    return true;
}
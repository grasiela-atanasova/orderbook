#pragma once

#include <map>
#include <list>
#include "order.hpp"
#include <vector>
#include <unordered_map>

using PriceLevel = std::list<Order>; //the list of Orders on each price level

struct OrderLocation{
    Side side;
    uint64_t price;
    PriceLevel::iterator it; //keeps where exactly each order is so we can cancel it at O(1)
};

class OrderBook{
public:
    void print_book () const;
    std::vector <Trade> add_order (const Order& incoming);
    bool cancel(uint64_t order_id);
    bool can_fully_fill(const Order& incoming) const;
    uint64_t quantity_at(Side side, uint64_t price) const;

private: //encapsulation
    std::map<uint64_t, PriceLevel> asks_;
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids_; 
    std::unordered_map<uint64_t, OrderLocation> index_;
};
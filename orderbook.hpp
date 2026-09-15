#pragma once

#include <map>
#include <list>
#include "order.hpp"

using PriceLevel = std::list<Order>; //the list of Orders per price

class OrderBook{
public:
    void add_limit_order(const Order& order);
    void print_book () const;
private: //encapsulation
    std::map<uint64_t, PriceLevel> asks_;
    std::map<uint64_t, PriceLevel, std::greater<int64_t>> bids_; 
};
#pragma once
#include <cstdint>

enum class Side{
    Buy,
    Sell
};

enum class OrderType{
    Limit,
    Market
};

struct Order{
    uint64_t id;
    Side side;
    OrderType type;
    uint64_t price;
    uint64_t quantity;
    uint64_t sequence;
};

struct Trade{
    uint64_t buy_order_id;
    uint64_t sell_order_id;
    uint64_t price;
    uint64_t quantity;
};
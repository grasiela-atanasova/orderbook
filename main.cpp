#include <iostream>

#include "order.hpp"

int main()
{
    Order o {1, Side::Buy, OrderType::Limit, 105, 50, 1};
    std::cout << "Order id: " << o.id;
    std::cout << " Price: " << o.price;
    std::cout << " Quantity: " << o.quantity << std::endl; 
    return 0;
}
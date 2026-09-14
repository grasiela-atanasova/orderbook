#include <bits/stdc++.h>
#include "order.hpp"
using namespace std;
int main()
{
    Order o {1, Side::Buy, OrderType::Limit, 105, 50, 1};
    cout << "Order id: " << o.id;
    cout << " Price: " << o.price;
    cout << " Quantity: " << o.quantity << endl; 
    return 0;
}
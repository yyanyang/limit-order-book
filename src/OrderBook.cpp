#include <iostream>
#include "OrderBook.h"

void OrderBook::addOrder(const Order& order)
{
    if(order.side == Side::buy)
    {
        auto& orders = bids_[order.price];
        auto it = orders.insert(orders.end(), order);
        orderLookup_[order.id] = it;
    }
    else
    {
        auto& orders = asks_[order.price];
        auto it = orders.insert(orders.end(), order);
        orderLookup_[order.id] = it;
    }
}

ostream& operator<<(ostream& os, const OrderBook& ob)
{
    os << "Limit Order Book\n";

    os << "\n******Buy orders********************" << std::endl;

    for(auto it = ob.bids_.begin(); it != ob.bids_.end(); ++it)
    {
        std::list<Order> orders = it->second;
        for(const Order& order : orders)
        {
            os << order.id << " "
                << (static_cast<int>(order.side) == 0 ? "buy" : "sell") << " "
                << order.price << " "
                << order.quantity << std::endl;
        }
    }

    os << "\n******Sell orders********************" << std::endl;

    for(auto it = ob.asks_.begin(); it != ob.asks_.end(); ++it)
    {
        std::list<Order> orders = it->second;
        for(const Order& order : orders)
        {
            os << order.id << " "
                << (static_cast<int>(order.side) == 0 ? "buy" : "sell") << " "
                << order.price << " "
                << order.quantity << std::endl;
        }
    }

    os << "\n******All orders********************" << std::endl;

    for(auto it = ob.orderLookup_.begin(); it != ob.orderLookup_.end(); ++it)
    {
        std::list<Order>::iterator itOrder = it->second;
        os << itOrder->id << " "
            << (static_cast<int>(itOrder->side) == 0 ? "buy" : "sell") << " "
            << itOrder->price << " "
            << itOrder->quantity << std::endl;        
    }
    return os;    
}

bool OrderBook::hasBestAsk() const
{
    return !asks_.empty();
}

bool OrderBook::hasBestBid() const
{
    return !bids_.empty();
}

double OrderBook::bestBid() const
{
    return bids_.begin()->first;
}

double OrderBook::bestAsk() const
{
    return asks_.begin()->first;
}

Order& OrderBook::bestAskOrder()
{
    return asks_.begin()->second.front();
}

Order& OrderBook::bestBidOrder()
{
    return bids_.begin()->second.front();
}

void OrderBook::removeBestAskOrder()
{
    auto levelIt = asks_.begin();
    auto& orders = levelIt->second;

    auto orderIt = orders.begin();

    orderLookup_.erase(orderIt->id);
    orders.erase(orderIt);

    if(orders.empty())
        asks_.erase(levelIt);
}

void OrderBook::removeBestBidOrder()
{
    auto levelIt = bids_.begin();
    auto& orders = levelIt->second;

    auto orderIt = orders.begin();

    orderLookup_.erase(orderIt->id);
    orders.erase(orderIt);

    if(orders.empty())
        bids_.erase(levelIt);
}

const Order& OrderBook::getOrder(OrderId id)
{
    auto it = orderLookup_[id];
    return *it;
}

bool OrderBook::cancelOrder(OrderId id)
{
    auto lookupIt = orderLookup_.find(id);

    if(lookupIt == orderLookup_.end())
        return false;

    auto itOrder = lookupIt->second;

    Side side = itOrder->side;
    double price = itOrder->price; 

    if(side == Side::buy)
    {
        auto levelIt = bids_.find(price);

        if(levelIt == bids_.end())
            return false;

        auto& orders = levelIt->second;
        orders.erase(itOrder);
        orderLookup_.erase(lookupIt);

        if(orders.empty())
            bids_.erase(levelIt);
    }
    else
    {
        auto levelIt = asks_.find(price);

        if(levelIt == asks_.end())
            return false;

        auto& orders = levelIt->second;
        orders.erase(itOrder);
        orderLookup_.erase(lookupIt);  
        
        if(orders.empty())
            asks_.erase(levelIt);
    }

    return true;
}

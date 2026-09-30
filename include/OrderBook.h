#pragma once
#ifndef _ORDER_BOOK_H_
#define _ORDER_BOOK_H_

#include <iostream>
#include <map>
#include <unordered_map>
#include <list>
#include <optional>
#include "order.h"

using std::ostream;
using OrderList = std::list<Order>;

class OrderBook
{
private:
    std::map<double, OrderList> asks_;
    std::map<double, OrderList, std::greater<>> bids_;

    std::unordered_map<OrderId, OrderList::iterator> orderLookup_;

public:
    friend ostream& operator<<(ostream& os, const OrderBook& ob);

    void addOrder(const Order& order);
    bool cancelOrder(OrderId id);

    const Order& getOrder(OrderId id);

    bool hasBestBid() const;
    bool hasBestAsk() const;

    double bestBid() const;
    double bestAsk() const;

    Order& bestBidOrder();
    Order& bestAskOrder();

    void removeBestBidOrder();
    void removeBestAskOrder();
};

#endif

#pragma once
#ifndef _MATCHING_ENGINE_H_
#define _MATCHING_ENGINE_H_

#include <vector>
#include "order.h"
#include "OrderBook.h"

class Trade;

class MatchingEngine
{
public:
    void submitOrder(const Order& order);
    const std::vector<Trade>& trades() const;

    bool hasBestBid() const;
    bool hasBestAsk() const;
    const Order& bestBidOrder();
    const Order& bestAskOrder();
    void printOrders() const;

private:
    void matchBuy(Order& incomingBuy);
    void matchSell(Order& incomingSell);

    OrderBook book_;
    std::vector<Trade> trades_;
};

struct Trade
{
    OrderId buyOrderId;
    OrderId sellOrderId;
    double price;
    uint32_t quantity;

    friend ostream& operator<<(ostream& os, const Trade& trade);
};

#endif

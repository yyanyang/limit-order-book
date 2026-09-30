#include "MatchingEngine.h"

void MatchingEngine::submitOrder(const Order& order)
{
    Order incomingOrder = order;
    if(order.side == Side::buy)
        matchBuy(incomingOrder);
    else
        matchSell(incomingOrder);
}

void MatchingEngine::matchBuy(Order& incomingBuy)
{
    std::vector<Trade> trades;

    while(incomingBuy.quantity > 0 && book_.hasBestAsk())
    {
        Order& resting = book_.bestAskOrder();

        if(resting.price > incomingBuy.price)
            break;

        uint32_t traded = std::min(incomingBuy.quantity, resting.quantity);

        trades_.push_back({incomingBuy.id, resting.id, resting.price, traded});

        // Execute trade
        incomingBuy.quantity -= traded;
        resting.quantity -= traded;

        if(resting.quantity == 0)
            book_.removeBestAskOrder();
    }

    if(incomingBuy.quantity > 0)
        book_.addOrder(incomingBuy);
}

void MatchingEngine::matchSell(Order& incomingSell)
{
    while(incomingSell.quantity > 0 && book_.hasBestBid())
    {
        auto& resting = book_.bestBidOrder();

        if(resting.price < incomingSell.price)
            break;

        uint32_t traded = std::min(incomingSell.quantity, resting.quantity);

        trades_.push_back({resting.id, incomingSell.id, resting.price, traded});

        // Execite trade
        incomingSell.quantity -= traded;
        resting.quantity -= traded;

        if(resting.quantity == 0)
            book_.removeBestBidOrder();
    }

    if(incomingSell.quantity > 0)
        book_.addOrder(incomingSell);
}

const std::vector<Trade>& MatchingEngine::trades() const
{
    return trades_;
}

bool MatchingEngine::hasBestBid() const
{
    return book_.hasBestBid();
}
bool MatchingEngine::hasBestAsk() const
{
    return book_.hasBestAsk();
}

const Order& MatchingEngine::bestBidOrder()
{
    return const_cast<Order&>(book_.bestBidOrder());
}
const Order& MatchingEngine::bestAskOrder()
{
    return const_cast<Order&>(book_.bestAskOrder());
}

void MatchingEngine::printOrders() const
{
    std::cout << book_;
}

ostream& operator<<(ostream& os, const Trade& trade)
{
    os << "Trade: " << trade.quantity << " @ " << trade.price 
        << " Buy #" << trade.buyOrderId 
        << " Sell #" << trade.sellOrderId << std::endl;
    return os;    
}

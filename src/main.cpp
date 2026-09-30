#include <iostream>
#include "MatchingEngine.h" 

int main()
{
    MatchingEngine engine;

    uint64_t orderNo = 1;   

    engine.submitOrder({orderNo++, Side::sell, 100.50, 100});
    engine.submitOrder({orderNo++, Side::sell, 100.40, 50});
    engine.submitOrder({orderNo++, Side::buy, 100.20, 100});
    engine.submitOrder({orderNo++, Side::buy, 100.10, 50});
    engine.submitOrder({orderNo++, Side::sell, 100.30, 40});
    engine.submitOrder({orderNo++, Side::buy, 100.00, 40});
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::buy, 100.50, 120});
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::sell, 100.00, 120});
    engine.printOrders();

    std::cout << "\n***** Trades *******************" << std::endl;
    std::vector<Trade> trades = engine.trades();
    for(Trade trade : trades)
        std::cout << trade;

    return 0;
}

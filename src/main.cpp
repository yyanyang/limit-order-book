#include <iostream>
#include <string_view>
#include <charconv>
#include <system_error>
#include "MatchingEngine.h" 

uint64_t toPriceTicks(const std::string_view price);

int main()
{
    MatchingEngine engine;

    uint64_t orderNo = 1;   

    engine.submitOrder({orderNo++, Side::sell, toPriceTicks("100.50"), 100});
    engine.submitOrder({orderNo++, Side::sell, toPriceTicks("100.40"), 50});
    engine.submitOrder({orderNo++, Side::buy, toPriceTicks("100.20"), 100});
    engine.submitOrder({orderNo++, Side::buy, toPriceTicks("100.10"), 50});
    engine.submitOrder({orderNo++, Side::sell, toPriceTicks("100.30"), 40});
    engine.submitOrder({orderNo++, Side::buy, toPriceTicks("100.00"), 40});
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::buy, toPriceTicks("100.50"), 120});
    std::cout << "\n***** Match buy *******************" << std::endl;
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::sell, toPriceTicks("100.00"), 120});
    std::cout << "\n***** Match sell *******************" << std::endl;
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::sell, toPriceTicks("100.05"), 100});
    std::cout << "\n***** Match sell *******************" << std::endl;
    engine.printOrders();

    engine.submitOrder({orderNo++, Side::buy, toPriceTicks("100.30"), 100});
    std::cout << "\n***** Match buy *******************" << std::endl;
    engine.printOrders();

    std::cout << "\n***** Trades *******************" << std::endl;
    std::vector<Trade> trades = engine.trades();
    for(Trade trade : trades)
        std::cout << trade;

    return 0;
}

uint64_t toPriceTicks(const std::string_view price)
{
    const auto dot = price.find('.');

    const auto wholePart = price.substr(0, dot);
    const auto fractionPart = dot == std::string_view::npos
                                ? std::string_view{}
                                : price.substr(dot + 1);

    if(wholePart.empty() || fractionPart.size() > 2)
        return false;

    uint64_t whole = 0;
    uint64_t fraction = 0;

    auto [ptr1, ec1] = std::from_chars(wholePart.data(),
                                       wholePart.data() + wholePart.size(), 
                                       whole);

    if(!fractionPart.empty())
    {
        auto [ptr2, ec2] = std::from_chars(fractionPart.data(),
                                           fractionPart.data() + fractionPart.size(),
                                           fraction);
    }

    if(whole > (UINT64_MAX - fraction) / 100)
        return false;

    uint32_t ticks = whole * 100 + fraction;
    return ticks;
}

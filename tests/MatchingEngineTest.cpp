#include <gtest/gtest.h>
#include "MatchingEngine.h"

TEST(MatchingEngineTest, PriceTimePriority)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::sell, 100, 10});
    engine.submitOrder({2, Side::sell, 100, 20});
    engine.submitOrder({3, Side::sell, 100, 30});

    engine.submitOrder({4, Side::buy, 100, 25});

    const auto& trades = engine.trades();

    ASSERT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].sellOrderId, 1);
    EXPECT_EQ(trades[0].quantity, 10);

    EXPECT_EQ(trades[1].sellOrderId, 2);
    EXPECT_EQ(trades[1].quantity, 15);
}

TEST(MatchingEngineTest, PartialFill)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::sell, 100, 100});
    engine.submitOrder({2, Side::buy, 100, 40});

    const auto& trades = engine.trades();

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 40);

    // Check remaining ask quantity
    EXPECT_EQ(engine.bestAskOrder().quantity, 60);
}

TEST(MatchingEngineTest, CompleteFill)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::sell, 100, 100});
    engine.submitOrder({2, Side::buy, 100, 100});

    EXPECT_EQ(engine.trades().size(), 1);
    EXPECT_FALSE(engine.hasBestAsk());
}

TEST(MatchingEngineTest, BuyDoesNotCrossHigherAsk)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::sell, 101, 50});
    engine.submitOrder({2, Side::buy, 100, 100});

    EXPECT_TRUE(engine.trades().empty());
}

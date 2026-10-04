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

TEST(MatchingEngineTest, BuyCrossLowerAsk)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::sell, 105, 100});
    engine.submitOrder({2, Side::sell, 104, 50});
    engine.submitOrder({3, Side::sell, 103, 40});
    engine.submitOrder({4, Side::buy, 105, 120});

    const auto& trades = engine.trades();

    EXPECT_EQ(trades.size(), 3);

    EXPECT_EQ(trades[0].sellOrderId, 3);
    EXPECT_EQ(trades[0].quantity, 40);

    EXPECT_EQ(trades[1].sellOrderId, 2);
    EXPECT_EQ(trades[1].quantity, 50);

    EXPECT_EQ(trades[2].sellOrderId, 1);
    EXPECT_EQ(trades[2].quantity, 30);
}

TEST(MatchingEngineTest, AskCrossHigherBuy)
{
    MatchingEngine engine;

    engine.submitOrder({1, Side::buy, 110, 30});
    engine.submitOrder({2, Side::buy, 100, 40});
    engine.submitOrder({3, Side::sell, 105, 100});

    const auto& trades = engine.trades();

    EXPECT_EQ(trades.size(), 1);

    EXPECT_EQ(trades[0].buyOrderId, 1);
    EXPECT_EQ(trades[0].quantity, 30);

    EXPECT_EQ(engine.bestAskOrder().id, 3);
    EXPECT_EQ(engine.bestAskOrder().quantity, 70);
    
    EXPECT_EQ(engine.bestBidOrder().id, 2);
    EXPECT_EQ(engine.bestBidOrder().quantity, 40);
}

#include <gtest/gtest.h>
#include "OrderBook.h"

TEST(OrderBookTest, AddBuyOrder)
{
    OrderBook book;

    book.addOrder({1, Side::buy, 100, 50});

    EXPECT_EQ(book.bestBid(), 100);
}

TEST(OrderBookTest, BestBidIsHighestPrice)
{
    OrderBook book;

    book.addOrder({1, Side::buy, 100, 10});
    book.addOrder({2, Side::buy, 101, 20});
    book.addOrder({3, Side::buy, 99, 30});

    EXPECT_EQ(book.bestBid(), 101);
}

TEST(OrderBookTest, BestAskIsLowestPrice)
{
    OrderBook book;

    book.addOrder({1, Side::sell, 102, 10});
    book.addOrder({2, Side::sell, 101, 20});
    book.addOrder({3, Side::sell, 103, 30});

    EXPECT_EQ(book.bestAsk(), 101);
}

TEST(OrderBookTest, CancelOrder)
{
    OrderBook book;

    book.addOrder({1, Side::buy, 100, 50});
    book.addOrder({2, Side::buy, 101, 20});

    EXPECT_TRUE(book.cancelOrder(2));
    EXPECT_EQ(book.bestBid(), 100);
}

TEST(OrderBookTest, CancelNonexistentOrder)
{
    OrderBook book;

    EXPECT_FALSE(book.cancelOrder(999));
}

TEST(OrderBookTest, EmptyBook)
{
    OrderBook book;

    EXPECT_FALSE(book.hasBestBid());
    EXPECT_FALSE(book.hasBestAsk());
}

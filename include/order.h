#pragma once
#ifndef _ORDER_H_
#define _ORDER_H_

#include <cstdint>

using OrderId = uint64_t;

enum class Side
{
    buy,
    sell
};

struct Order
{
    OrderId id;
    Side side;
    uint64_t price;
    uint64_t quantity;
};

#endif

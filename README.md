# C++ Limit Order Book

A simplified limit order book and matching engine implemented in modern C++20.

This project was developed as a personal project to strengthen modern C++ skills.

## Features

- Buy and sell limit orders
- Price-time priority matching
- Best bid / Best ask
- Order cancellation
- Partial and complete order fills
- Multiple price-level matching
- Trade generation
- Unit and integration tests using GoogleTest

## Design

The order book maintains separate bid and ask sides:

| Bids | Asks |
|------|------|
| **Higher price first** | **Lower price first** |
| Price 100 | Price 101 |
| ├── Order A | ├── Order D |
| ├── Order B | └── Order E |
| └── Order C | Price 102 |
| | └── Order F |

Orders at the same price are maintained in time priority.

The implementation uses:
- std::map for price levels
- std::list for orders at each price level
- std::unordered_map for fast orderID lookup

#### Conceptually:
    std::map<double, std::list<Order>> asks_;
    std::map<double, std::list<Order>, std::greater<>> bids_;

    std::unordered_map<OrderId, std::list<Order>::iterator> orderLookup_;

The matching engine processes incoming orders against the best available prices.

## Matching Rules
### Buy orders

A buy order can match the lowest priced sell orders where:
ask price <= bid price
### Sell orderes

A sell order can match the highest priced buy orders where:
bid price >= ask price

Within the same price level, orders are matched according to time priority.
### Example

Suppose the ask side contains:

| Price | Quantity |
|------|------|
| 103 | 20 |
| 104 | 30 |
| 105 | 50 |

An incoming buy order for 80 units at 105 can match:


20 @ 103

30 @ 104

30 @ 105

The remaining ask quantity at 105 is:

20

The incoming order is completely filled.

### Project Structure
```text
LOB/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Order.h
│   ├── OrderBook.h
│   └── MatchingEngine.h
├── src/
│   ├── main.cpp
│   ├── OrderBook.cpp
│   └── MatchingEngine.cpp
└── tests/
    ├── OrderBookTest.cpp
    └── MatchingEngineTest.cpp
```
### Building
This project uses CMake and requires a C++20 compatible compiler.

Build:
cmake --build build
### Running
The example application can be run with:

./build/lob_app.exe

On Windows with Visual Studio, the executable will normally be generated under the appropriate configuration directory, for example:

build/Debug/lob_app.exe
    
### Testing
The project uses GoogleTest.

Build the tests:

cmake --build build

Run all tests:

./build/lob_tests.exe

The tests cover:
- Adding orders
- Best bid and best ask
- Order cancellation
- Exact fills
- Partial fills
- Multiple price level fills
- Price priority
- Time priority
- Orders that cannot be matched
### Design Considerations
#### Order Cancellation

Orders are indexed by orderID using an unordered_map, allowing an order to be removed without searching through the entire book.

#### Price Time Priority
The price level is selected using the std::map ordering, while the std::list preserves the arrival order of orders at the same price.

#### Future Improvements
Possible extension include:
- Market orders
- Multithreaded data feed simulation

#### Technologies
- C++20
- STL
- CMake
- GoogleTest
- Git

#### Disclaimer

This is an educational project. It is not intended for live trading or production financial use.

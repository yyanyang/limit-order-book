cmake_minimum_required(VERSION 3.22)

project(LOB VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------
# LOB library
# -------------------------------

add_library(lob STATIC
        src/OrderBook.cpp 
        src/MatchingEngine.cpp)

target_include_directories(lob PUBLIC ${PROJECT_SOURCE_DIR}/include)

# -------------------------------
# Main application
# -------------------------------

add_executable(lob_app src/main.cpp)

target_link_libraries(lob_app PRIVATE lob)

# --------------------------------
# Tests
# --------------------------------

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest 
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(lob_tests
        ${PROJECT_SOURCE_DIR}/tests/OrderBookTest.cpp 
        ${PROJECT_SOURCE_DIR}/tests/MatchingEngineTest.cpp
)

target_link_libraries(lob_tests PRIVATE lob GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(lob_tests)
#include <fstream>

#include <gtest/gtest.h>

#include "config.h"
#include "parse/parse.hh"

// Allow namespace pollution in tests for convenience
using namespace kraken;
using namespace kraken::parse;

namespace {

std::vector<Order> parse_file(std::string const& input_name) {
    using namespace std::literals;
    auto input
        = std::ifstream{CMAKE_SOURCE_DIR + "/data/inputs/"s + input_name};
    return parse_orders(input);
}

} // namespace

TEST(parse_orders, empty) {
    auto const expected = std::vector<Order>{};
    ASSERT_EQ(parse_file("empty.in.csv"), expected);
}

TEST(parse_orders, balance_book_1) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::BID,
                   UserOrderId{3}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::ASK,
                   UserOrderId{103}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{4}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{104}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-1.in.csv"), expected);
}

TEST(parse_orders, balance_book_2) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::ASK,
                   UserOrderId{103}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-2.in.csv"), expected);
}

TEST(parse_orders, balance_book_3) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::BID,
                   UserOrderId{103}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-3.in.csv"), expected);
}

TEST(parse_orders, balance_book_4) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{10}, Quantity{20}, Side::ASK,
                   UserOrderId{103}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-4.in.csv"), expected);
}

TEST(parse_orders, balance_book_5) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{11}, Quantity{20}, Side::BID,
                   UserOrderId{3}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-5.in.csv"), expected);
}

TEST(parse_orders, balance_book_6) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        CancelOrder{User{1}, UserOrderId{1}},
        CancelOrder{User{2}, UserOrderId{102}},
        FlushOrder{}};
    ASSERT_EQ(parse_file("balanced-book-6.in.csv"), expected);
}

TEST(parse_orders, balance_book_7) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        CancelOrder{User{1}, UserOrderId{2}},
        CancelOrder{User{2}, UserOrderId{101}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-7.in.csv"), expected);
}

TEST(parse_orders, balance_book_8) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        CancelOrder{User{1}, UserOrderId{1}},
        CancelOrder{User{2}, UserOrderId{101}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-8.in.csv"), expected);
}

TEST(parse_orders, balance_book_9) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{103}},
        CancelOrder{User{2}, UserOrderId{103}},
        CancelOrder{User{2}, UserOrderId{102}},
        CancelOrder{User{1}, UserOrderId{2}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("balanced-book-9.in.csv"), expected);
}

TEST(parse_orders, shallow_ask) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"VAL"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{2}, Symbol{"VAL"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"VAL"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{1}, Symbol{"VAL"}, Price{11}, Quantity{100}, Side::BID,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"VAL"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{103}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("shallow-ask.in.csv"), expected);
}

TEST(parse_orders, shallow_bid) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"AAPL"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"AAPL"}, Price{12}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"AAPL"}, Price{11}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{2}, Symbol{"AAPL"}, Price{10}, Quantity{100}, Side::ASK,
                   UserOrderId{103}},
        TradeOrder{User{1}, Symbol{"AAPL"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{3}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("shallow-bid.in.csv"), expected);
}

TEST(parse_orders, tighten_spread_through_new_limit_orders) {
    auto const expected = std::vector<Order>{
        TradeOrder{User{1}, Symbol{"IBM"}, Price{10}, Quantity{100}, Side::BID,
                   UserOrderId{1}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{16}, Quantity{100}, Side::ASK,
                   UserOrderId{2}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{9}, Quantity{100}, Side::BID,
                   UserOrderId{101}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{15}, Quantity{100}, Side::ASK,
                   UserOrderId{102}},
        TradeOrder{User{2}, Symbol{"IBM"}, Price{11}, Quantity{100}, Side::BID,
                   UserOrderId{103}},
        TradeOrder{User{1}, Symbol{"IBM"}, Price{14}, Quantity{100}, Side::ASK,
                   UserOrderId{3}},
        FlushOrder{},
    };
    ASSERT_EQ(parse_file("tighten-spread-through-new-limit-orders.in.csv"),
              expected);
}

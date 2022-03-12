#include <fstream>

#include <gtest/gtest.h>

#include "config.h"
#include "csv/read-csv.hh"
#include "engine/csv-engine-listener.hh"
#include "engine/engine.hh"
#include "parse/parse.hh"

// Allow namespace pollution in tests for convenience
using namespace kraken;
using namespace kraken::csv;
using namespace kraken::engine;
using namespace kraken::parse;

namespace {

void do_compare(std::istream& input, std::istream& expected_output,
                CrossBehaviour behaviour) {
    auto listener = std::make_shared<CsvEngineListener>();
    auto engine = Engine{listener, behaviour};

    engine.process_orders(parse_orders(input));

    auto expected = read_csv(expected_output, CsvHeader::KEEP);

    ASSERT_EQ(listener->output(), expected);
}

void compare(std::string const& test_name) {
    using namespace std::literals;
    auto input = std::ifstream{CMAKE_SOURCE_DIR + "/data/inputs/"s + test_name
                               + ".in.csv"};
    auto expected_output_file = std::ifstream{
        CMAKE_SOURCE_DIR + "/data/outputs/"s + test_name + ".out.csv"};

    do_compare(input, expected_output_file, CrossBehaviour::REJECT);
}

void compare_trades(std::string const& test_name) {
    using namespace std::literals;
    auto input = std::ifstream{CMAKE_SOURCE_DIR + "/data/inputs/"s + test_name
                               + ".in.csv"};
    auto expected_output_file = std::ifstream{
        CMAKE_SOURCE_DIR + "/data/outputs/"s + test_name + ".trades.out.csv"};

    do_compare(input, expected_output_file, CrossBehaviour::MATCH);
}

} // namespace

TEST(engine, empty) {
    compare("empty");
}

TEST(engine, only_one) {
    compare("only-one");
}

TEST(engine, only_one_cancelled) {
    compare("only-one-cancelled");
}

TEST(engine, two) {
    compare("two");
}

TEST(engine, balanced_book_1) {
    compare("balanced-book-1");
}

TEST(engine, balanced_book_2) {
    compare("balanced-book-2");
}

TEST(engine, balanced_book_3) {
    compare("balanced-book-3");
}

TEST(engine, balanced_book_4) {
    compare("balanced-book-4");
}

TEST(engine, balanced_book_5) {
    compare("balanced-book-5");
}

TEST(engine, balanced_book_6) {
    compare("balanced-book-6");
}

TEST(engine, balanced_book_7) {
    compare("balanced-book-7");
}

TEST(engine, balanced_book_8) {
    compare("balanced-book-8");
}

TEST(engine, balanced_book_9) {
    compare("balanced-book-9");
}

TEST(engine, shallow_ask) {
    compare("shallow-ask");
}

TEST(engine, shallow_bid) {
    compare("shallow-bid");
}

TEST(engine, tighten_spread_through_new_limit_orders) {
    compare("tighten-spread-through-new-limit-orders");
}

TEST(engine_trade, balanced_book_3) {
    compare_trades("balanced-book-3");
}

TEST(engine_trade, shallow_ask) {
    compare_trades("shallow-ask");
}

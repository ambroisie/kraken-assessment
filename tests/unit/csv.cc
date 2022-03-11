#include <gtest/gtest.h>

#include "csv/read-csv.hh"

// Allow namespace pollution in tests for convenience
using namespace kraken::csv;

TEST(csv, empty) {
    auto const expected = csv_type{};
    ASSERT_EQ(read_csv(""), expected);
}

TEST(csv, skip_header) {
    auto const expected = csv_type{};
    ASSERT_EQ(read_csv("a,b,c"), expected);
}

TEST(csv, do_not_skip_header) {
    auto const expected = csv_type{
        {"a", "b", "c"},
    };
    ASSERT_EQ(read_csv("a,b,c", CsvHeader::KEEP), expected);
}

TEST(csv, single_line) {
    auto const expected = csv_type{
        {"1", "2", "3"},
    };
    ASSERT_EQ(read_csv("a,b,c\n1,2,3"), expected);
}

TEST(csv, single_line_with_header) {
    auto const expected = csv_type{
        {"a", "b", "c"},
        {"1", "2", "3"},
    };
    ASSERT_EQ(read_csv("a,b,c\n1,2,3", CsvHeader::KEEP), expected);
}

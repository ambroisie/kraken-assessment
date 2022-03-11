#include <sstream>

#include <gtest/gtest.h>

#include "csv/read-csv.hh"
#include "csv/write-csv.hh"

// Allow namespace pollution in tests for convenience
using namespace kraken::csv;

TEST(read_csv, empty) {
    auto const expected = csv_type{};
    ASSERT_EQ(read_csv(""), expected);
}

TEST(read_csv, skip_header) {
    auto const expected = csv_type{};
    ASSERT_EQ(read_csv("a,b,c"), expected);
}

TEST(read_csv, do_not_skip_header) {
    auto const expected = csv_type{
        {"a", "b", "c"},
    };
    ASSERT_EQ(read_csv("a,b,c", CsvHeader::KEEP), expected);
}

TEST(read_csv, single_line) {
    auto const expected = csv_type{
        {"1", "2", "3"},
    };
    ASSERT_EQ(read_csv("a,b,c\n1,2,3"), expected);
}

TEST(read_csv, single_line_with_header) {
    auto const expected = csv_type{
        {"a", "b", "c"},
        {"1", "2", "3"},
    };
    ASSERT_EQ(read_csv("a,b,c\n1,2,3", CsvHeader::KEEP), expected);
}

TEST(write_csv, empty) {
    auto const csv = csv_type{};
    auto const expected = "";
    auto output = std::ostringstream{};
    write_csv(output, csv);
    ASSERT_EQ(output.str(), expected);
}

TEST(write_csv, single_line) {
    auto const csv = csv_type{
        {"a", "b", "c"},
    };
    auto const expected = "a,b,c\n";
    auto output = std::ostringstream{};
    write_csv(output, csv);
    ASSERT_EQ(output.str(), expected);
}

TEST(write_csv, multiple_lines) {
    auto const csv = csv_type{
        {"a", "b", "c"},
        {"1", "2", "3"},
    };
    auto const expected = "a,b,c\n1,2,3\n";
    auto output = std::ostringstream{};
    write_csv(output, csv);
    ASSERT_EQ(output.str(), expected);
}

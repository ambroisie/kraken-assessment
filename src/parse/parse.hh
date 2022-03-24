#pragma once

#include <iosfwd>
#include <stdexcept>
#include <vector>

#include "book/order.hh"
#include "csv/csv.hh"

namespace kraken::parse {

struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/// Convert a single CSV input line to an `Order`.
Order parse_single_order(csv::csv_line_type const& order);

/// Parse orders from an input stream as CSV.
std::vector<Order> parse_orders(std::istream& input);

} // namespace kraken::parse

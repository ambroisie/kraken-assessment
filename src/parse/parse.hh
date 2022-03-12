#pragma once

#include <iosfwd>
#include <stdexcept>
#include <vector>

#include "order.hh"

namespace kraken::parse {

struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::vector<Order> parse_orders(std::istream& input);

} // namespace kraken::parse

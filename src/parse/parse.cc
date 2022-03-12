#include "parse.hh"

#include <string>

#include <cassert>

#include "csv/read-csv.hh"

namespace kraken::parse {

namespace {

Side parse_side(std::string_view side) {
    if (side == "B") {
        return Side::BID;
    } else if (side == "S") {
        return Side::ASK;
    } else {
        throw ParseError("Not a valid side");
    }
}

TradeOrder trade_from_raw(csv::csv_line_type const& raw_order) {
    assert(raw_order[0] == "N");

    if (raw_order.size() != 7) {
        throw ParseError("Not a valid trade order");
    }

    auto const user = User{std::stoi(raw_order[1])};
    auto const symbol = Symbol{raw_order[2]};
    auto const price = Price{std::stoi(raw_order[3])};
    auto const quantity = Quantity{std::stoi(raw_order[4])};
    auto const side = Side{parse_side(raw_order[5])};
    auto const id = UserOrderId{std::stoi(raw_order[6])};

    return TradeOrder{
        user, symbol, price, quantity, side, id,
    };
}

CancelOrder cancel_from_raw(csv::csv_line_type const& raw_order) {
    assert(raw_order[0] == "C");

    if (raw_order.size() != 3) {
        throw ParseError("Not a valid cancel order");
    }

    auto const user = User{std::stoi(raw_order[1])};
    auto const id = UserOrderId{std::stoi(raw_order[2])};

    return CancelOrder{
        user,
        id,
    };
}

} // namespace

std::vector<Order> parse_orders(std::istream& input) {
    auto const raw_orders = read_csv(input, kraken::csv::CsvHeader::KEEP);
    auto orders = std::vector<Order>{};

    for (auto const& raw_order : raw_orders) {
        if (raw_order[0] == "N") {
            orders.emplace_back(trade_from_raw(raw_order));
        } else if (raw_order[0] == "C") {
            orders.emplace_back(cancel_from_raw(raw_order));
        } else if (raw_order[0] == "F") {
            orders.emplace_back(FlushOrder{});
        } else {
            throw ParseError("Not a valid order");
        }
    }

    return orders;
}

} // namespace kraken::parse

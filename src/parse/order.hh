#pragma once

#include <string>
#include <variant>

#include "utils/strong-type.hh"

namespace kraken::parse {

/// Which side the order is on.
enum class Side {
    /// An offer to sell.
    BID,
    /// An offer to buy.
    ASK,
};

struct PriceTag;
/// A strongly-typed price value.
using Price = utils::StrongType<int, PriceTag>;

struct QuantityTag;
/// A strongly-typed quantity value.
using Quantity = utils::StrongType<int, PriceTag>;

struct SymbolTag;
/// A strongly-typed symbol value.
using Symbol = utils::StrongType<std::string, SymbolTag>;

struct UserTag;
/// A strongly-typed user id value.
using User = utils::StrongType<int, UserTag>;

struct UserOrderIdTag;
/// A strongly-typed user/order id value.
using UserOrderId = utils::StrongType<int, UserOrderIdTag>;

/// DTO to represent an order
struct TradeOrder {
    /// User of the order.
    User user;
    /// Symbol of the order.
    Symbol symbol;
    /// Price of the order.
    Price price;
    /// Quantity of the order.
    Quantity quantity;
    /// Side of the order.
    Side side;
    /// User/order id.
    UserOrderId id;

    auto operator<=>(TradeOrder const&) const = default;
};

struct CancelOrder {
    /// User of the order.
    User user;
    /// User/order id to cancel.
    UserOrderId id;

    auto operator<=>(CancelOrder const&) const = default;
};

struct FlushOrder {
    auto operator<=>(FlushOrder const&) const = default;
};

using Order = std::variant<TradeOrder, CancelOrder, FlushOrder>;

} // namespace kraken::parse

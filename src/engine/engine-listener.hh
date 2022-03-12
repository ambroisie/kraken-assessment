#pragma once

#include <optional>

#include "book/order.hh"

namespace kraken::engine {

/// Listener interface which is called by the engine on trading events.
struct EngineListener {
    virtual ~EngineListener();

    /// Called when a new trade or cancel order has been acknowledged.
    virtual void on_acknowledgement(User user, UserOrderId id) = 0;

    /// Called when an order is invalid.
    virtual void on_bad_order(User user, UserOrderId id) = 0;

    /// Called when the top of the book changes.
    virtual void on_top_of_book_change(Side side, Price price,
                                       Quantity total_quantity)
        = 0;
    /// Called when the top of the book changes, no orders remaining on a side.
    virtual void on_top_of_book_change(Side side) = 0;

    /// Called when an order is rejected by the engine for crossing the book.
    virtual void on_rejection(User user, UserOrderId id) = 0;

    /// Called when an order is matched by the engine.
    virtual void on_match(User user_buy, UserOrderId id_bid, User user_sell,
                          UserOrderId id_ask, Price price, Quantity quantity)
        = 0;

    /// Called when a flush order is processed.
    virtual void on_flush() = 0;
};

} // namespace kraken::engine

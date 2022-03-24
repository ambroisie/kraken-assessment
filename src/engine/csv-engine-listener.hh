#pragma once

#include <iosfwd>

#include "csv/csv.hh"

#include "engine-listener.hh"

namespace kraken::engine {

struct CsvEngineListener : EngineListener {
    CsvEngineListener();

    virtual ~CsvEngineListener();

    csv::csv_type const& output() const;
    csv::csv_type& output();

    /// Called when a new trade or cancel order has been acknowledged.
    void on_acknowledgement(User user, UserOrderId id) override;

    /// Called when an order is invalid.
    void on_bad_order(User user, UserOrderId id) override;

    /// Called when the top of the book changes.
    void on_top_of_book_change(Side side, Price price,
                               Quantity total_quantity) override;
    /// Called when the top of the book changes, no orders remaining on a side.
    void on_top_of_book_change(Side side) override;

    /// Called when an order is rejected by the engine for crossing the book.
    void on_rejection(User user, UserOrderId id) override;

    /// Called when an order is matched by the engine.
    void on_match(User user_buy, UserOrderId id_bid, User user_sell,
                  UserOrderId id_ask, Price price, Quantity quantity) override;

    /// Called when a flush order is processed.
    void on_flush() override;

private:
    csv::csv_type output_{};
};

}; // namespace kraken::engine

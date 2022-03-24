#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "book/order.hh"

namespace kraken::engine {

/// Which behaviour on cross orders.
enum class CrossBehaviour {
    /// Reject the crossing order.
    REJECT,
    /// Make the trade with the matching order(s).
    MATCH,
};

struct CallbackOnTopOfBookChange;
struct EngineListener;

/// Matching engine which processes orders and keeps the book up-to-date.
struct Engine {
    Engine(std::shared_ptr<EngineListener> listener,
           CrossBehaviour cross_behaviour = CrossBehaviour::REJECT);

    /// Process a single order, triggerring the listener appropriately.
    void process_single_order(Order const& order);

    /// Process orders, triggerring the listener on each event.
    void process_orders(std::vector<Order> const& orders);

private:
    void operator()(TradeOrder const& trade_order);
    void operator()(CancelOrder const& cancel_order);
    void operator()(FlushOrder const& flush_order);

    std::shared_ptr<EngineListener> listener_;
    CrossBehaviour cross_behaviour_;

    // Symbol, price, side are implicit given the way the book is represented
    struct OrderMetaData {
        User user;
        UserOrderId id;
        Quantity quantity;
    };

    friend struct CallbackOnTopOfBookChange;

    // Sorted by price, then by time
    template <typename Order>
    using instrument_side_data = std::multimap<Price, OrderMetaData, Order>;

    std::map<Symbol, instrument_side_data<std::greater<void>>> bids_;
    std::map<Symbol, instrument_side_data<std::less<void>>> asks_;

    // Map cancel information to allow faster lookups
    struct CancelReverseInfo {
        Side side;
        Symbol symbol;
        Price price;
    };

    std::map<CancelOrder, CancelReverseInfo> cancel_reverse_info_;
};

} // namespace kraken::engine

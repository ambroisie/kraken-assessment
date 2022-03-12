#include "engine.hh"

#include <algorithm>
#include <utility>

#include <cassert>

#include "engine-listener.hh"

namespace kraken::engine {

// A RAII wrapper to ensure the top-of-book callback is made
struct CallbackOnTopOfBookChange {
    CallbackOnTopOfBookChange(Symbol symbol, Engine& engine)
        : symbol_(symbol), engine_(engine) {
        top_info_ = calculate_top();
    }

    ~CallbackOnTopOfBookChange() {
        auto const new_top = calculate_top();

        // Sanity check: both sides of the book are not changing at once
        assert(!(new_top.asks != top_info_.asks
                 && new_top.bids != top_info_.bids));

        if (top_info_.asks != new_top.asks) {
            if (new_top.asks) {
                engine_.listener_->on_top_of_book_change(
                    Side::ASK, new_top.asks->first, new_top.asks->second);
            } else {
                engine_.listener_->on_top_of_book_change(Side::ASK);
            }
        } else if (top_info_.bids != new_top.bids) {
            if (new_top.bids) {
                engine_.listener_->on_top_of_book_change(
                    Side::BID, new_top.bids->first, new_top.bids->second);
            } else {
                engine_.listener_->on_top_of_book_change(Side::BID);
            }
        }
    }

private:
    struct TopInfo {
        std::optional<std::pair<Price, Quantity>> asks;
        std::optional<std::pair<Price, Quantity>> bids;
    };

    TopInfo calculate_top() const {
        auto info = TopInfo{};

        if (auto const bid_it = engine_.bids_.find(symbol_);
            bid_it != engine_.bids_.end()) {
            auto const& [_, bid_map] = *bid_it;

            if (bid_map.size() > 0) {
                auto const price = bid_map.begin()->first;
                int quantity = 0;
                auto const [begin, end] = bid_map.equal_range(price);

                for (auto it = begin; it != end; ++it) {
                    quantity += int(it->second.quantity);
                }

                info.bids = {price, Quantity(quantity)};
            }
        }

        if (auto const ask_it = engine_.asks_.find(symbol_);
            ask_it != engine_.asks_.end()) {
            auto const& [_, ask_map] = *ask_it;

            if (ask_map.size() > 0) {
                auto const price = ask_map.begin()->first;
                int quantity = 0;
                auto const [begin, end] = ask_map.equal_range(price);

                for (auto it = begin; it != end; ++it) {
                    quantity += int(it->second.quantity);
                }

                info.asks = {price, Quantity(quantity)};
            }
        }

        return info;
    }

    Symbol symbol_;
    Engine& engine_;
    TopInfo top_info_{};
};

Engine::Engine(std::shared_ptr<EngineListener> listener)
    : listener_(listener) {}

void Engine::process_orders(std::vector<Order> const& orders) {
    for (auto const& order : orders) {
        std::visit([this](auto const& trade_order) { (*this)(trade_order); },
                   order);
    }
}

void Engine::operator()(TradeOrder const& trade_order) {
    // Set-up automatic call-back in case top-of-book changes
    auto _ = CallbackOnTopOfBookChange(trade_order.symbol, *this);

    // NOTE: some amount of repetition/mirroring
    switch (trade_order.side) {
    case Side::ASK:
        if (auto bid_map_it = bids_.find(trade_order.symbol);
            bid_map_it != bids_.end()) {
            auto& [_, bid_map] = *bid_map_it;
            if (bid_map.size() > 0
                && bid_map.begin()->first >= trade_order.price) {
                // FIXME: handle matching if enabled
                listener_->on_rejection(trade_order.user, trade_order.id);
                return;
            }
        }
        asks_[trade_order.symbol].insert(
            {trade_order.price, OrderMetaData{trade_order.user, trade_order.id,
                                              trade_order.quantity}});
        break;
    case Side::BID:
        if (auto ask_map_it = asks_.find(trade_order.symbol);
            ask_map_it != asks_.end()) {
            auto& [_, ask_map] = *ask_map_it;
            if (ask_map.size() > 0
                && ask_map.begin()->first <= trade_order.price) {
                // FIXME: handle matching if enabled
                listener_->on_rejection(trade_order.user, trade_order.id);
                return;
            }
        }
        bids_[trade_order.symbol].insert(
            {trade_order.price, OrderMetaData{trade_order.user, trade_order.id,
                                              trade_order.quantity}});
        break;
    }

    // Record reverse-lookup info for faster cancelling
    cancel_reverse_info_.insert({
        {
            trade_order.user,
            trade_order.id,
        },
        {
            trade_order.side,
            trade_order.symbol,
            trade_order.price,
        },
    });

    listener_->on_acknowledgement(trade_order.user, trade_order.id);
}

void Engine::operator()(CancelOrder const& cancel_order) {
    auto const matches_order = [&](auto const& data) {
        return data.second.user == cancel_order.user
            && data.second.id == cancel_order.id;
    };

    if (auto it = cancel_reverse_info_.find(cancel_order);
        it != cancel_reverse_info_.end()) {
        auto const& [_, info] = *it;

        // Set-up automatic call-back in case top-of-book changes
        auto __ = CallbackOnTopOfBookChange(info.symbol, *this);

        switch (info.side) {
        case Side::ASK: {
            auto& ask_map = asks_[info.symbol];
            auto const [begin, end] = ask_map.equal_range(info.price);
            auto it = std::find_if(begin, end, matches_order);
            if (it == end) {
                listener_->on_bad_order(cancel_order.user, cancel_order.id);
                return;
            }
            ask_map.erase(it);
            break;
        }
        case Side::BID: {
            auto& bid_map = bids_[info.symbol];
            auto const [begin, end] = bid_map.equal_range(info.price);
            auto it = std::find_if(begin, end, matches_order);
            if (it == end) {
                listener_->on_bad_order(cancel_order.user, cancel_order.id);
                return;
            }
            bid_map.erase(it);
            break;
        }
        }
        cancel_reverse_info_.erase(it);
        listener_->on_acknowledgement(cancel_order.user, cancel_order.id);
    }
}

void Engine::operator()(FlushOrder const&) {
    bids_.clear();
    asks_.clear();
    cancel_reverse_info_.clear();

    listener_->on_flush();
}

} // namespace kraken::engine

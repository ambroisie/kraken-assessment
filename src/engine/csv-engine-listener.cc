#include "csv-engine-listener.hh"

namespace kraken::engine {

CsvEngineListener::CsvEngineListener() = default;

CsvEngineListener::~CsvEngineListener() = default;

csv::csv_type const& CsvEngineListener::output() const {
    return output_;
}

void CsvEngineListener::on_acknowledgement(User user, UserOrderId id) {
    output_.emplace_back(csv::csv_line_type{
        "A",
        std::to_string(int(user)),
        std::to_string(int(id)),
    });
}

void CsvEngineListener::on_bad_order(User, UserOrderId) {
    // Nothing to do
}

void CsvEngineListener::on_top_of_book_change(Side side, Price price,
                                              Quantity quantity) {
    output_.emplace_back(csv::csv_line_type{
        "B",
        side == Side::BID ? "B" : "S",
        std::to_string(int(price)),
        std::to_string(int(quantity)),
    });
}

void CsvEngineListener::on_top_of_book_change(Side side) {
    output_.emplace_back(csv::csv_line_type{
        "B",
        side == Side::BID ? "B" : "S",
        "-",
        "-",
    });
}

void CsvEngineListener::on_rejection(User user, UserOrderId id) {
    output_.emplace_back(csv::csv_line_type{
        "R",
        std::to_string(int(user)),
        std::to_string(int(id)),
    });
}

void CsvEngineListener::on_match(User user_buy, UserOrderId id_bid,
                                 User user_sell, UserOrderId id_ask,
                                 Price price, Quantity quantity) {
    output_.emplace_back(csv::csv_line_type{
        "T",
        std::to_string(int(user_buy)),
        std::to_string(int(id_bid)),
        std::to_string(int(user_sell)),
        std::to_string(int(id_ask)),
        std::to_string(int(price)),
        std::to_string(int(quantity)),
    });
}

void CsvEngineListener::on_flush() {
    // Nothing to do
}

} // namespace kraken::engine

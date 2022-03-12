#include <iostream>

#include "csv/write-csv.hh"
#include "engine/csv-engine-listener.hh"
#include "engine/engine.hh"
#include "parse/parse.hh"

int main() {
    auto const orders = kraken::parse::parse_orders(std::cin);

    auto listener = std::make_shared<kraken::engine::CsvEngineListener>();

    auto engine = kraken::engine::Engine(listener);

    engine.process_orders(orders);

    kraken::csv::write_csv(std::cout, listener->output());
}

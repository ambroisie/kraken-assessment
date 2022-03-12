#include <iostream>

#include "csv/write-csv.hh"
#include "engine/csv-engine-listener.hh"
#include "engine/engine.hh"
#include "parse/parse.hh"

int main(int argc, char** argv) {
    auto cross_behaviour = kraken::engine::CrossBehaviour::REJECT;

    if (argc > 1) {
        using namespace std::literals;
        if (argv[1] == "--enable-trade"sv) {
            cross_behaviour = kraken::engine::CrossBehaviour::MATCH;
        }
    }

    auto const orders = kraken::parse::parse_orders(std::cin);

    auto listener = std::make_shared<kraken::engine::CsvEngineListener>();

    auto engine = kraken::engine::Engine(listener, cross_behaviour);

    engine.process_orders(orders);

    kraken::csv::write_csv(std::cout, listener->output());
}

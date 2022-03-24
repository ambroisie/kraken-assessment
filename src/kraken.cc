#include <iostream>

#include "csv/read-csv.hh"
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

    auto listener = std::make_shared<kraken::engine::CsvEngineListener>();
    auto engine = kraken::engine::Engine(listener, cross_behaviour);

    for (std::string line; std::getline(std::cin, line);) {
        auto const order = kraken::parse::parse_single_order(
            kraken::csv::read_csv_line(line));

        engine.process_single_order(order);

        auto& output = listener->output();
        kraken::csv::write_csv(std::cout, output);
        output.resize(0);
    }
}

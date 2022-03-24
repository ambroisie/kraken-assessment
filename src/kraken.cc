#include <iostream>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>

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

    // Up to 512 pending orders.
    auto pending_orders = boost::lockfree::spsc_queue<kraken::Order>(512);

    auto writer = std::jthread([&](std::stop_token stop_token) {
        auto listener = std::make_shared<kraken::engine::CsvEngineListener>();
        auto engine = kraken::engine::Engine(listener, cross_behaviour);

        while (true) {
            auto order = kraken::Order();
            while (!pending_orders.pop(order)) {
                // FIXME: busy wait
                // Check that we didn't miss an order between last 'pop' and
                // stop request, just in case.
                if (stop_token.stop_requested() && pending_orders.empty()) {
                    return;
                }
            }
            engine.process_single_order(order);
            auto& output = listener->output();
            kraken::csv::write_csv(std::cout, output);
            output.resize(0);
        }
    });

    auto reader = std::jthread([&]() {
        for (std::string line; std::getline(std::cin, line);) {
            auto const order = kraken::parse::parse_single_order(
                kraken::csv::read_csv_line(line));
            while (!pending_orders.push(order)) {
                // FIXME: busy wait
            }
        }
        // EOF, bring process orders and bring
        writer.request_stop();
    });

    reader.join();
}

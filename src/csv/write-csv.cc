#include "write-csv.hh"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace kraken::csv {

void write_csv(std::ostream& output, csv_type const& csv) {
    for (const auto& line : csv) {
        bool first = true;
        for (const auto& atom : line) {
            output << (first ? "" : ",") << atom;
            first = false;
        };
        output << '\n';
    }
}

} // namespace kraken::csv

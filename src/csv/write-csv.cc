#include "write-csv.hh"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace kraken::csv {

void write_csv(std::ostream& output, csv_type const& csv) {
    for (const auto& line : csv) {
        std::ranges::copy(line,
                          std::ostream_iterator<std::string>{output, ","});
    }
}

} // namespace kraken::csv

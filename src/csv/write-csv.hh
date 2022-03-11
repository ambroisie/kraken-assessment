#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "csv/csv.hh"

namespace kraken::csv {

/// Write raw CSV data to an output-stream.
void write_csv(std::ostream& output, csv_type const& csv);

} // namespace kraken::csv

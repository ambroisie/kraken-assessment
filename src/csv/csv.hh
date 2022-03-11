#pragma once

#include <string>
#include <vector>

namespace kraken::csv {

/// Represent a raw CSV line as a vector of strings.
using csv_line_type = std::vector<std::string>;

/// Represent a raw CSV file as a vector of raw CSV lines
using csv_type = std::vector<csv_line_type>;

} // namespace kraken::csv

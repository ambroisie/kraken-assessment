#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace kraken::csv {

/// Should the first line of the CSV file be kept or ignored.
enum class CsvHeader {
    /// Ignored.
    SKIP,
    /// Kepts.
    KEEP,
};

/// Represent a raw CSV line as a vector of strings.
using csv_line_type = std::vector<std::string>;

/// Represent a raw CSV file as a vector of raw CSV lines
using csv_type = std::vector<csv_line_type>;

/// Parse a CSV file from an input-stream, return a vector of parsed lines.
csv_type read_csv(std::istream& input, CsvHeader header = CsvHeader::SKIP);

/// Convenience function which reads CSV from a string.
csv_type read_csv(std::string const& input, CsvHeader header = CsvHeader::SKIP);

} // namespace kraken::csv

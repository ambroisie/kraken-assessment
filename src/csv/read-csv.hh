#pragma once

#include <iosfwd>

#include "csv/csv.hh"

namespace kraken::csv {

/// Should the first line of the CSV file be kept or ignored.
enum class CsvHeader {
    /// Ignored.
    SKIP,
    /// Kepts.
    KEEP,
};

/// Parse a CSV file from an input-stream, return a vector of parsed lines.
csv_type read_csv(std::istream& input, CsvHeader header = CsvHeader::SKIP);

/// Convenience function which reads CSV from a string.
csv_type read_csv(std::string const& input, CsvHeader header = CsvHeader::SKIP);

} // namespace kraken::csv

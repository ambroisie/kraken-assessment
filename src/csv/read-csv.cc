#include "read-csv.hh"

#include <sstream>

namespace kraken::csv {

csv_line_type read_csv_line(std::string const& line) {
    auto parsed = csv_line_type{};

    auto input = std::istringstream(line);
    for (std::string atom; std::getline(input, atom, ',');) {
        parsed.emplace_back(std::move(atom));
    }

    return parsed;
}

csv_type read_csv(std::istream& input, CsvHeader header) {
    auto parsed = std::vector<csv_line_type>{};

    bool first = true;
    for (std::string line; std::getline(input, line);) {
        if (first && header == CsvHeader::SKIP) {
            first = false;
            continue;
        }

        parsed.emplace_back(read_csv_line(std::move(line)));
    }

    return parsed;
}

csv_type read_csv(std::string const& input, CsvHeader header) {
    auto input_stream = std::istringstream(input);
    return read_csv(input_stream, header);
}

} // namespace kraken::csv

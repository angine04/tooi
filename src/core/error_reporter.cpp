#include "tooi/core/error_reporter.h"

#include <iostream>
#include <string>
#include <algorithm> // For std::max

namespace tooi {
namespace core {

void ErrorReporter::report_at(int line, int column, int length, const std::string& source_line, const std::string& message) {
    // Ensure column and length are at least 1 for calculations
    int display_column = std::max(1, column);
    int display_length = std::max(1, length);

    // Print basic error info
    // TODO: Add filename context later if needed
    std::cerr << "Error [line " << line << ":" << display_column << "]: " << message << std::endl;

    // Print the source line
    std::cerr << "  | " << source_line << std::endl;

    // Print the carets (^^^^)
    std::cerr << "  | ";
    // Add spaces for padding up to the column
    for (int i = 1; i < display_column; ++i) {
        // Handle tabs correctly if they affect column alignment
        if (i -1 < source_line.length() && source_line[i - 1] == '\t') {
             std::cerr << '\t'; // Output tab to maintain alignment
        } else {
             std::cerr << ' ';
        }
    }
    // Add the carets
    for (int i = 0; i < display_length; ++i) {
        std::cerr << '^';
    }
    std::cerr << std::endl;

    had_error_ = true;
}

bool ErrorReporter::had_error() const {
    return had_error_;
}

void ErrorReporter::reset() {
    had_error_ = false;
}

} // namespace core
} // namespace tooi 
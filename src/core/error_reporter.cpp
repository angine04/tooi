#include "tooi/core/error_reporter.h"

#include <iostream>
#include <string>
#include <algorithm> // For std::max

namespace {
// ANSI Color Codes (simple version)
const char* const COLOR_RED_BOLD = "\x1B[1;31m";
const char* const COLOR_RESET = "\x1B[0m";
} // anonymous namespace

namespace tooi {
namespace core {

void ErrorReporter::report_at(int line, int column, int length, const std::string& source_line, const std::string& message) {
    // Ensure column and length are at least 1 for calculations
    int display_column = std::max(1, column);
    int display_length = std::max(1, length);

    // Print basic error info (colored)
    std::cerr << COLOR_RED_BOLD // Start Red + Bold
              << "Error [line " << line << ":" << display_column << "]: "
              << message
              << COLOR_RESET // Reset color before newline
              << std::endl;

    // Print the source line (normal color)
    std::cerr << "  | " << source_line << std::endl;

    // Print the carets (colored red)
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
    // Add the carets (colored)
    std::cerr << COLOR_RED_BOLD; // Start Red + Bold for carets
    for (int i = 0; i < display_length; ++i) {
        std::cerr << '^';
    }
    std::cerr << COLOR_RESET // Reset color after carets
              << std::endl;

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
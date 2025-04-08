#include "tooi/core/error_reporter.h"
#include "tooi/cli/colors.h"
#include "tooi/core/error_registry.h" // Added for ErrorRegistry
#include <fmt/format.h>            // Added for fmt::format
#include <sstream>
#include <iostream>

namespace tooi {
namespace core {

// Prints the source context (source line and carets) for a reported error.
// Assumes the primary error message line has already been printed by the caller.
void ErrorReporter::print_error(int line, int column, int length, const std::string& source_line, const std::string& formatted_error_line) {
    using namespace tooi::cli::colors; // Using declaration for convenience
    // Ensure column and length are at least 1 for calculations
    int display_column = std::max(1, column);
    int display_length = std::max(1, length);

    // 1. Print the already formatted error line received from the caller
    std::cerr << formatted_error_line << std::endl;

    // 2. Print the source line
    std::cerr << "  | " << source_line << std::endl;

    // 3. Print the carets
    std::cerr << "  | ";
    // Add spaces for padding up to the column, handling tabs
    for (int i = 0; i < display_column - 1; ++i) {
        if (i < source_line.length() && source_line[i] == '\t') {
             std::cerr << '\t'; // Preserve tab width
        } else {
             std::cerr << ' '; // Add a single space
        }
    }
    // Add the carets (keep them red for errors/fatal/internal)
    std::cerr << BOLD_RED;
    for (int i = 0; i < display_length; ++i) {
        std::cerr << '^';
    }
    std::cerr << RESET << std::endl;

    // NOTE: had_error_ flag is set by the caller (report_at/report_general)
    // had_error_ = true; // REMOVED
}

bool ErrorReporter::had_error() const {
    return had_error_;
}

void ErrorReporter::reset() {
    had_error_ = false;
}

} // namespace core
} // namespace tooi 
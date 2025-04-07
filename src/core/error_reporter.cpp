#include "tooi/core/error_reporter.h"
#include "tooi/cli/colors.h"
#include "tooi/core/error_registry.h" // Added for ErrorRegistry
#include <fmt/format.h>            // Added for fmt::format
#include <sstream>

namespace tooi {
namespace core {

void ErrorReporter::print_error(int line, int column, int length, const std::string& source_line, const std::string& message) {
    using namespace tooi::cli::colors; // Using declaration for convenience
    // Ensure column and length are at least 1 for calculations
    int display_column = std::max(1, column);
    int display_length = std::max(1, length);

    // Print basic error info
    std::cerr << BOLD_RED
              << "Error [line " << line << ":" << display_column << "]: "
              << message
              << RESET
              << std::endl;

    // Print the source line
    std::cerr << "  | " << source_line << std::endl;

    // Print the carets
    std::cerr << "  | ";
    // Add spaces for padding up to the column
    for (int i = 1; i < display_column; ++i) {
        if (i -1 < source_line.length() && source_line[i - 1] == '\t') {
             std::cerr << '\t';
        } else {
             std::cerr << ' ';
        }
    }
    // Add the carets
    std::cerr << BOLD_RED;
    for (int i = 0; i < display_length; ++i) {
        std::cerr << '^';
    }
    std::cerr << RESET
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
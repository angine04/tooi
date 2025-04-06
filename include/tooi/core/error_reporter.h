#pragma once

#include <string>
#include <iostream> // For std::cerr

namespace tooi {
namespace core {

/**
 * @brief Handles reporting and tracking of errors during compilation/interpretation.
 */
class ErrorReporter {
public:
    ErrorReporter() = default;

    /**
     * @brief Reports an error message with source context.
     *
     * Prints the line number, column, error message, the source line,
     * and a caret pointing to the error location.
     *
     * @param line The line number (1-based).
     * @param column The column number (1-based) where the error starts.
     * @param length The number of characters the error spans (for carets).
     * @param source_line The full text of the source code line containing the error.
     * @param message The error message.
     */
    void report_at(int line, int column, int length, const std::string& source_line, const std::string& message);

    /**
     * @brief Checks if any errors have been reported.
     * @return True if report() has been called at least once, false otherwise.
     */
    bool had_error() const;

    /**
     * @brief Resets the error state.
     */
    void reset();

private:
    bool had_error_ = false;
};

} // namespace core
} // namespace tooi 
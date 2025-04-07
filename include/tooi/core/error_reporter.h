#pragma once

#include "tooi/core/error_info.h"      // Added for ErrorCode
#include "tooi/core/error_registry.h"  // Added: Include definition for ErrorRegistry
#include <fmt/core.h>    // Added for fmt::format_args (forward declare might work too)
#include <fmt/format.h>  // Include the full format library for fmt::format
#include <iostream>      // For std::cerr
#include <string>

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
    void print_error(int line, int column, int length, const std::string& source_line,
                     const std::string& message);

    /**
     * @brief Reports an error message using an ErrorCode and formatting arguments.
     *
     * Retrieves error information from ErrorRegistry, formats the message,
     * and then calls the original report_at to display it with context.
     *
     * @tparam Args Variadic arguments for the error message format string.
     * @param line The line number (1-based).
     * @param column The column number (1-based) where the error starts.
     * @param length The number of characters the error spans.
     * @param source_line The full text of the source code line.
     * @param code The ErrorCode representing the specific error.
     * @param args The arguments to be formatted into the error message.
     */
    template <typename... Args>
    void report_at(int line, int column, int length, const std::string& source_line, ErrorCode code,
                   Args&&... args) {
        try {
            // 1. Get error information from the registry
            const ErrorInfo& error_info = ErrorRegistry::instance().get_info(code);

            // 2. Format the message using fmt::format directly
            std::string formatted_message =
                fmt::format(fmt::runtime(error_info.message_fmt), std::forward<Args>(args)...);

            // 3. Print the error message with context
            print_error(line, column, length, source_line, formatted_message);
            had_error_ = true;

        } catch (const ErrorRegistryError& e) {
            // Handle cases where the error code itself is invalid or registry failed
            std::string fallback_message =
                fmt::format("Internal Error: Failed to report error code {} - {}",
                            static_cast<int>(code), e.what());
            print_error(line, column, length, source_line, fallback_message);
            had_error_ = true;
        } catch (const fmt::format_error& e) {
            // Handle errors during message formatting (e.g., wrong number/type of args)
            std::string fallback_message =
                fmt::format("Internal Error: Failed to format message for error code {} - {}",
                            static_cast<int>(code), e.what());
            print_error(line, column, length, source_line, fallback_message);
            had_error_ = true;
        }
    }

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

}  // namespace core
}  // namespace tooi
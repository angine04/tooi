#pragma once

#include <string>
#include <iostream> // For std::cerr, std::endl
#include "tooi/core/error_info.h"      // Added for ErrorCode
#include "tooi/core/error_registry.h"  // Added: Include definition for ErrorRegistry
#include "tooi/cli/colors.h"           // Added for color constants
#include <fmt/core.h>    // Added for fmt::format_args (forward declare might work too)
#include <fmt/format.h>  // Include the full format library for fmt::format

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
        using namespace tooi::cli::colors;
        std::string prefix;
        std::string color_code;
        ErrorSeverity severity;

        try {
            // 1. Get error information from the registry
            const ErrorInfo& error_info = ErrorRegistry::instance().get_info(code);
            severity = error_info.severity;

            // 2. Determine prefix word and color based on severity
            switch (severity) {
                case ErrorSeverity::Warning:
                    prefix = "Warning"; // No trailing colon/space
                    color_code = BOLD_YELLOW;
                    break;
                case ErrorSeverity::Fatal:
                    prefix = "Fatal"; // No trailing colon/space
                    color_code = BOLD_RED;
                    break;
                case ErrorSeverity::Internal:
                    prefix = "Internal Error"; // No trailing colon/space
                    color_code = BOLD_MAGENTA;
                    break;
                case ErrorSeverity::Error: // Fallthrough intended
                default:
                    prefix = "Error"; // No trailing colon/space
                    color_code = BOLD_RED;
                    break;
            }

            // 3. Format the core message
            std::string core_message =
                fmt::format(fmt::runtime(error_info.message_fmt), std::forward<Args>(args)...);
            
            // 4. Format the final error line in the desired format: Severity [line row:col]: msg
            std::string final_error_line =
                fmt::format("{}{} [line {}:{}]: {}{}", 
                            color_code, prefix, line, column, core_message, RESET);
            
            // 5. Print the error message with context
            print_error(line, column, length, source_line, final_error_line);
            had_error_ = true; // Set error flag regardless of severity for now

        } catch (const ErrorRegistryError& e) {
            // Handle cases where the error code itself is invalid or registry failed
            severity = ErrorSeverity::Internal;
            prefix = "Internal Error"; // No trailing colon/space
            color_code = BOLD_MAGENTA; // Consistent color for internal
            std::string fallback_core_message =
                fmt::format("Failed to report error code {} - {}",
                            static_cast<int>(code), e.what());
            // Format fallback message in the desired format
            std::string final_fallback_line =
                fmt::format("{}{} [line {}:{}]: {}{}", 
                            color_code, prefix, line, column, fallback_core_message, RESET);
            print_error(line, column, length, source_line, final_fallback_line);
            had_error_ = true;
        } catch (const fmt::format_error& e) {
            // Handle errors during message formatting (e.g., wrong number/type of args)
            severity = ErrorSeverity::Internal;
            prefix = "Internal Error"; // No trailing colon/space
            color_code = BOLD_MAGENTA; // Consistent color for internal
            std::string fallback_core_message =
                fmt::format("Failed to format message for error code {} - {}",
                            static_cast<int>(code), e.what());
            // Format fallback message in the desired format
            std::string final_fallback_line =
                fmt::format("{}{} [line {}:{}]: {}{}", 
                            color_code, prefix, line, column, fallback_core_message, RESET);
            print_error(line, column, length, source_line, final_fallback_line);
            had_error_ = true;
        }
    }

    /**
     * @brief Reports a general error message not associated with a specific code location.
     *
     * Retrieves error information, formats the message, and prints it to stderr.
     *
     * @tparam Args Variadic arguments for the error message format string.
     * @param code The ErrorCode representing the specific error.
     * @param args The arguments to be formatted into the error message.
     */
    template<typename... Args>
    void report_general(ErrorCode code, Args&&... args) {
        using namespace tooi::cli::colors;
        std::string prefix;
        std::string color_code;
        ErrorSeverity severity;

        try {
            // 1. Get error information from the registry
            const ErrorInfo& error_info = ErrorRegistry::instance().get_info(code);
            severity = error_info.severity;

           // 2. Determine prefix and color based on severity
            switch (severity) {
                case ErrorSeverity::Warning:
                    prefix = "Warning: ";
                    color_code = BOLD_YELLOW;
                    break;
                case ErrorSeverity::Fatal:
                    prefix = "Fatal: ";
                    color_code = BOLD_RED;
                    break;
                 case ErrorSeverity::Internal:
                    prefix = "Internal Error: ";
                    color_code = BOLD_MAGENTA;
                    break;
                case ErrorSeverity::Error:
                default:
                    prefix = "Error: ";
                    color_code = BOLD_RED;
                    break;
            }

            // 3. Format the core message
            std::string core_message =
                fmt::format(fmt::runtime(error_info.message_fmt), std::forward<Args>(args)...);

            // 4. Print the general error message directly
            std::cerr << color_code << prefix << core_message << RESET << std::endl;
            had_error_ = true; // Set error flag

        } catch (const ErrorRegistryError& e) {
            // Handle cases where the error code itself is invalid or registry failed
            severity = ErrorSeverity::Internal;
            prefix = "Internal Error: ";
            color_code = BOLD_MAGENTA;
            std::string fallback_core_message =
                fmt::format("Failed to report error code {} - {}",
                            static_cast<int>(code), e.what());
            std::cerr << color_code << prefix << fallback_core_message << RESET << std::endl;
            had_error_ = true;
        } catch (const fmt::format_error& e) {
            // Handle errors during message formatting (e.g., wrong number/type of args)
            severity = ErrorSeverity::Internal;
            prefix = "Internal Error: ";
            color_code = BOLD_MAGENTA;
            std::string fallback_core_message =
                fmt::format("Failed to format message for error code {} - {}",
                            static_cast<int>(code), e.what());
             std::cerr << color_code << prefix << fallback_core_message << RESET << std::endl;
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
#pragma once

#include <stdexcept>  // For std::runtime_error
#include <string>

namespace tooi {
namespace core {

/**
 * @brief Enum representing all possible error codes in the Tooi compiler/interpreter.
 */
enum class ErrorCode {
    // --- Scanner Errors ---
    Scanner_InvalidCharacter,
    Scanner_UnterminatedString,
    Scanner_MalformedNumber_DecimalRequiresDigit,  // e.g., "1."
    Scanner_MalformedNumber_MultipleDecimals,      // e.g., "1.2.3"
    Scanner_InvalidNumericSuffix,                  // e.g., "123xyz"
    Scanner_SuffixRequiresNoDecimal_Int,           // e.g., "1.2i32"
    // Note: The rule forbidding 'f' with decimal was removed, keeping enum value commented out for
    // history
    // Scanner_SuffixRequiresNoDecimal_Float,    // e.g., "1.2f"
    Scanner_NumberParseError_Invalid,     // std::invalid_argument during conversion
    Scanner_NumberParseError_OutOfRange,  // std::out_of_range during conversion

    // --- Parser Errors ---
    // TODO: Add parser error codes
    Parser_UnexpectedToken,
    Parser_ExpectedExpression,
    // ... other parser errors

    // --- Semantic Errors ---
    // TODO: Add semantic error codes

    // --- Runtime Errors ---
    // TODO: Add runtime error codes

    // --- General/Internal Errors ---
    Registry_UnknownErrorCode  // Fallback if an unknown code is requested
};

/**
 * @brief Structure holding information about a specific error type.
 */
struct ErrorInfo {
    ErrorCode id;
    std::string code_str;     // e.g., "E_SCANNER_INVALID_SUFFIX"
    std::string message_fmt;  // Format string (compatible with {fmt})
    std::string description;  // Longer explanation of the error
};

// Custom exception class for registry errors (e.g., unknown error code)
class ErrorRegistryError : public std::runtime_error {
public:
    explicit ErrorRegistryError(const std::string& message) : std::runtime_error(message) {}
};

}  // namespace core
}  // namespace tooi

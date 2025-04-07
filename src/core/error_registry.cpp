#include "tooi/core/error_registry.h"
#include <stdexcept> // For std::out_of_range used by map.at()

namespace tooi {
namespace core {

// Initialize static members for the singleton
std::unique_ptr<ErrorRegistry> ErrorRegistry::instance_ = nullptr;
std::once_flag ErrorRegistry::init_flag_;

// Private constructor
ErrorRegistry::ErrorRegistry() {
    initialize_registry();
}

// Singleton instance getter
ErrorRegistry& ErrorRegistry::instance() {
    std::call_once(init_flag_, []() {
        instance_.reset(new ErrorRegistry());
        // Note: using reset(new ...) instead of make_unique because constructor is private
    });
    return *instance_;
}

// Get ErrorInfo by code
const ErrorInfo& ErrorRegistry::get_info(ErrorCode code) const {
    try {
        return registry_map_.at(code);
    } catch (const std::out_of_range& oor) {
        // Provide a fallback error message if the requested code is unknown
        try {
             return registry_map_.at(ErrorCode::Registry_UnknownErrorCode);
        } catch (...) {
            // If even the fallback is missing (shouldn't happen), throw a basic error
             throw ErrorRegistryError("Requested ErrorCode not found and fallback Registry_UnknownErrorCode is also missing.");
        }
        // Ideally, log the original unknown code here as well
        // For now, just return the generic unknown code error info
    }
}

// Initialize the registry map (hardcoded for now)
void ErrorRegistry::initialize_registry() {
    // --- Scanner Errors ---
    registry_map_[ErrorCode::Scanner_InvalidCharacter] = {
        ErrorCode::Scanner_InvalidCharacter, "E_SCANNER_INVALID_CHAR",
        "Invalid character encountered: '{}'.",
        "The scanner found a character that is not part of the defined Tooi language syntax or valid character sets (like UTF-8)."
    };
    registry_map_[ErrorCode::Scanner_UnterminatedString] = {
        ErrorCode::Scanner_UnterminatedString, "E_SCANNER_UNTERM_STR",
        "Unterminated string literal.",
        "String literals must be closed with a matching double quote (\"). The end of the file or line was reached before the closing quote was found."
    };
    registry_map_[ErrorCode::Scanner_MalformedNumber_DecimalRequiresDigit] = {
        ErrorCode::Scanner_MalformedNumber_DecimalRequiresDigit, "E_SCANNER_DECIMAL_REQ_DIGIT",
        "Decimal point must be followed by digits.",
        "A decimal point (.) in a number must be immediately followed by at least one digit (0-9)."
    };
     registry_map_[ErrorCode::Scanner_MalformedNumber_MultipleDecimals] = {
        ErrorCode::Scanner_MalformedNumber_MultipleDecimals, "E_SCANNER_MULTIPLE_DECIMALS",
        "Invalid number format: multiple decimal points.",
        "Numeric literals can contain at most one decimal point."
    };
    registry_map_[ErrorCode::Scanner_InvalidNumericSuffix] = {
        ErrorCode::Scanner_InvalidNumericSuffix, "E_SCANNER_INVALID_SUFFIX",
        "Invalid numeric suffix: '{}'.",
        "The suffix used for the numeric literal is not recognized. Valid suffixes include i, u, f, d, i32, i64, u32, u64."
    };
     registry_map_[ErrorCode::Scanner_SuffixRequiresNoDecimal_Int] = {
        ErrorCode::Scanner_SuffixRequiresNoDecimal_Int, "E_SCANNER_INT_SUFFIX_DECIMAL",
        "Cannot use integer suffix '{}' with a decimal point.",
        "Integer type suffixes (like i, u, i32, i64, u32, u64) cannot be applied to numbers containing a decimal point."
    };
    registry_map_[ErrorCode::Scanner_NumberParseError_Invalid] = {
        ErrorCode::Scanner_NumberParseError_Invalid, "E_SCANNER_PARSE_INVALID",
        "Invalid number format for specified type (suffix: '{}').",
        "The numeric literal could not be parsed into the target type indicated by the suffix (or default type). Check the format."
    };
     registry_map_[ErrorCode::Scanner_NumberParseError_OutOfRange] = {
        ErrorCode::Scanner_NumberParseError_OutOfRange, "E_SCANNER_PARSE_RANGE",
        "Number out of range for specified type (suffix: '{}').",
        "The numeric literal's value is too large or too small to fit into the target type indicated by the suffix (or default type)."
    };

    // --- General/Internal Errors ---
    registry_map_[ErrorCode::Registry_UnknownErrorCode] = {
        ErrorCode::Registry_UnknownErrorCode, "E_REGISTRY_UNKNOWN",
        "Unknown internal error code encountered: {}.", // Placeholder for the integer value of the code
        "An internal error occurred where an undefined error code was requested from the error registry."
    };

    // TODO: Add definitions for Parser, Semantic, Runtime errors here later
}


} // namespace core
} // namespace tooi

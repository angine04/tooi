#include "catch2.hpp"
#include "tooi/core/error_info.h"
#include "tooi/core/error_registry.h"

#include <string>
#include <stdexcept> // For std::runtime_error

using tooi::core::ErrorCode;
using tooi::core::ErrorInfo;
using tooi::core::ErrorRegistry;
using tooi::core::ErrorRegistryError;
using tooi::core::ErrorSeverity;

TEST_CASE("ErrorRegistry Singleton", "[error][registry]") {
    SECTION("Instance is obtainable and consistent") {
        ErrorRegistry& instance1 = ErrorRegistry::instance();
        ErrorRegistry& instance2 = ErrorRegistry::instance();
        REQUIRE(&instance1 == &instance2); // Check if both references point to the same object
    }
}

TEST_CASE("ErrorRegistry get_info", "[error][registry]") {
    ErrorRegistry& registry = ErrorRegistry::instance();

    SECTION("Retrieve info for known scanner error codes") {
        // Test a few known scanner error codes
        ErrorCode code1 = ErrorCode::Scanner_InvalidCharacter;
        const ErrorInfo* info1 = nullptr;
        INFO("Checking ErrorCode: Scanner_InvalidCharacter (" << static_cast<int>(code1) << ")");
        REQUIRE_NOTHROW(info1 = &registry.get_info(code1));
        REQUIRE(info1 != nullptr);
        REQUIRE(info1->id == code1);
        REQUIRE_FALSE(info1->code_str.empty());
        REQUIRE_FALSE(info1->message_fmt.empty());

        ErrorCode code2 = ErrorCode::Scanner_UnterminatedString;
        const ErrorInfo* info2 = nullptr;
        INFO("Checking ErrorCode: Scanner_UnterminatedString (" << static_cast<int>(code2) << ")");
        REQUIRE_NOTHROW(info2 = &registry.get_info(code2));
        REQUIRE(info2 != nullptr);
        REQUIRE(info2->id == code2);
        REQUIRE_FALSE(info2->code_str.empty());
        REQUIRE_FALSE(info2->message_fmt.empty());

        ErrorCode code3 = ErrorCode::Scanner_MalformedNumber_MultipleDecimals;
        const ErrorInfo* info3 = nullptr;
        INFO("Checking ErrorCode: Scanner_MalformedNumber_MultipleDecimals (" << static_cast<int>(code3) << ")");
        REQUIRE_NOTHROW(info3 = &registry.get_info(code3));
        REQUIRE(info3 != nullptr);
        REQUIRE(info3->id == code3);
        REQUIRE_FALSE(info3->code_str.empty());
        REQUIRE_FALSE(info3->message_fmt.empty());
    }
    
    SECTION("Retrieve info for a known general error code") {
         ErrorCode code = ErrorCode::Registry_UnknownErrorCode;
         const ErrorInfo* info = nullptr;
         INFO("Checking ErrorCode: Registry_UnknownErrorCode (" << static_cast<int>(code) << ")");
         REQUIRE_NOTHROW(info = &registry.get_info(code));
         REQUIRE(info != nullptr);
         REQUIRE(info->id == code);
         REQUIRE_FALSE(info->code_str.empty());
         REQUIRE_FALSE(info->message_fmt.empty());
    }

    SECTION("Throws exception for an unknown/invalid error code") {
        ErrorCode unknown_code = static_cast<ErrorCode>(-999);
        INFO("Checking throw for unknown ErrorCode: " << static_cast<int>(unknown_code));
        
        REQUIRE_THROWS_AS(registry.get_info(unknown_code), ErrorRegistryError);
        
        try {
            registry.get_info(unknown_code);
        } catch (const ErrorRegistryError& e) {
            std::string actual_message = e.what();
            std::string expected_msg_part = "Unknown ErrorCode requested: -999";
            CAPTURE(unknown_code, expected_msg_part, actual_message); // Capture variables
            REQUIRE(actual_message.find(expected_msg_part) != std::string::npos);
        } catch (...) {
            FAIL("Threw unexpected exception type.");
        }
    }
}

TEST_CASE("ErrorInfo Structure", "[error][info]") {
    // Basic check for ErrorInfo construction and members
    // This is less critical as it's mostly a data holder, but good for completeness
    ErrorInfo info = {
        ErrorCode::Scanner_InvalidCharacter,
        ErrorSeverity::Error,
        "E_SCANNER_INVALID_CHAR",
        "Invalid character '{0}' found.",
        "The scanner encountered a character it does not recognize."
    };

    REQUIRE(info.id == ErrorCode::Scanner_InvalidCharacter);
    REQUIRE(info.severity == ErrorSeverity::Error);
    REQUIRE(info.code_str == "E_SCANNER_INVALID_CHAR");
    REQUIRE(info.message_fmt == "Invalid character '{0}' found.");
    REQUIRE(info.description == "The scanner encountered a character it does not recognize.");
}

// We might add tests for ErrorReporter later, potentially mocking dependencies.
// For now, focus is on the registry and info structures. 
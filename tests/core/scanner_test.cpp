#include "catch2.hpp"
#include "tooi/core/scanner.h" // Assuming scanner header exists
#include "tooi/core/token.h"
#include "tooi/core/error_reporter.h" // Need the base class definition
#include "tooi/core/error_info.h"     // Corrected: Include for ErrorCode
#include <vector>
#include <string>
#include <iostream> // For potential debugging output inside reporter

// Simple concrete ErrorReporter for testing purposes
class TestErrorReporter : public tooi::core::ErrorReporter {
public:
    // Override the virtual base method print_error to suppress output
    void print_error(int line, int column, int length, const std::string& source_line, const std::string& formatted_error_line) override {
        // Intentionally empty to suppress console output from report_at
        // Note: report_general prints directly to cerr, this won't stop that.
        // We rely on the base class templates setting the protected had_error_ flag.
    }

    // We don't need to override report_at or report_general because:
    // 1. The base versions are templates and cannot be overridden.
    // 2. The base templates set the protected had_error_ flag directly.
    // 3. We override print_error() to stop report_at from printing.

    // Use the virtual had_error() and reset() from the base class.
    // These correctly access the protected had_error_ member.
    using ErrorReporter::had_error;
    using ErrorReporter::reset;

    // No private members needed here, had_error_ is inherited as protected.
};

// Test case 1: Basic Scanner initialization and empty input
TEST_CASE("Scanner Basic Initialization", "[scanner]") {
    TestErrorReporter error_reporter;
    std::string empty_source = "";
    tooi::core::Scanner scanner(empty_source, error_reporter);
    
    INFO("Checking empty source: No errors, 1 token (EOF), empty lexeme.");
    std::vector<tooi::core::Token> tokens = scanner.scan_tokens();
    
    REQUIRE_FALSE(error_reporter.had_error());
    REQUIRE(tokens.size() == 1); // Only EOF token
    REQUIRE(tokens[0].type == tooi::core::TokenType::END_OF_FILE);
    REQUIRE(tokens[0].lexeme == "");
}

// Test case 2: Comprehensive token coverage
TEST_CASE("Scanner Comprehensive Token Coverage", "[scanner]") {
    TestErrorReporter error_reporter;
    
    // Source code covering all token types
    std::string source = R"(
        // Single-character tokens
        ( ) { } [ ] , . - + ; / * @ # $ ? : ^ % & | ~
        
        // One or two character tokens
        ! != = == > >= < <= :: -> => >>
        
        // Literals
        hello123 "string literal" 'another string' `raw string`
        42 3.14 42i32 3.14f 42u64
        
        // Keywords
        if else for while done skip
        true false nil and or not
        add export with self as call
        let set new do be of in
        public private runnable pure param
        
        // Type keywords
        int float byte string bool uint proto
        int32 int64 uint32 uint64 float32 float64
    )";
    
    tooi::core::Scanner scanner(source, error_reporter);
    std::vector<tooi::core::Token> tokens = scanner.scan_tokens();
    
    REQUIRE_FALSE(error_reporter.had_error());
    
    // Create a map of token types to their counts
    std::unordered_map<tooi::core::TokenType, int> token_counts;
    for (const auto& token : tokens) {
        token_counts[token.type]++;
    }
    
    // Verify all token types are present using SECTIONs for better reporting
    SECTION("Single-character tokens are present") {
        INFO("Checking single-char tokens: ( ) { } [ ] , . - + ; / * @ # $ ? : ^ % & | ~");
        REQUIRE(token_counts[tooi::core::TokenType::LEFT_PAREN] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::RIGHT_PAREN] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::LEFT_BRACE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::RIGHT_BRACE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::LEFT_BRACKET] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::RIGHT_BRACKET] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::COMMA] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::DOT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::MINUS] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PLUS] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::SEMICOLON] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::SLASH] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::ASTERISK] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::AT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::HASHTAG] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::DOLLAR] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::QUESTION] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::COLON] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::CARET] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PERCENT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::AMPERSAND] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PIPE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::TILDE] > 0);
    }
    
    SECTION("One or two character tokens are present") {
        INFO("Checking one/two-char tokens: ! != = == > >= < <= :: -> => >>");
        REQUIRE(token_counts[tooi::core::TokenType::BANG] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::BANG_EQUAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::EQUAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::EQUAL_EQUAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::GREATER] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::GREATER_EQUAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::LESS] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::LESS_EQUAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::COLON_COLON] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::MINUS_GREATER] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::EQUAL_GREATER] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::GREATER_GREATER] > 0);
    }
    
    SECTION("Literals are present") {
        INFO("Checking literals: IDENTIFIER_LITERAL, STRING_LITERAL, NUMBER_LITERAL");
        REQUIRE(token_counts[tooi::core::TokenType::IDENTIFIER_LITERAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::STRING_LITERAL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::NUMBER_LITERAL] > 0);
    }
    
    SECTION("General keywords are present") {
        INFO("Checking general keywords: IF, ELSE, FOR, WHILE, DONE, SKIP, TRUE, FALSE, NIL, AND, OR, NOT, ADD, EXPORT, WITH, SELF, AS, CALL, LET, SET, NEW, DO, BE, OF, IN, PUBLIC, PRIVATE, RUNNABLE, PURE, PARAM");
        REQUIRE(token_counts[tooi::core::TokenType::IF] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::ELSE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::FOR] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::WHILE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::DONE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::SKIP] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::TRUE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::FALSE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::NIL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::AND] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::OR] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::NOT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::ADD] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::EXPORT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::WITH] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::SELF] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::AS] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::CALL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::LET] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::SET] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::NEW] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::DO] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::BE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::OF] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::IN] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PUBLIC] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PRIVATE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::RUNNABLE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PURE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PARAM] > 0);
    }
    
    SECTION("Type keywords are present") {
        INFO("Checking type keywords: INT, FLOAT, BYTE, STRING, BOOL, UINT, PROTO, INT32, INT64, UINT32, UINT64, FLOAT32, FLOAT64");
        REQUIRE(token_counts[tooi::core::TokenType::INT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::FLOAT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::BYTE] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::STRING] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::BOOL] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::UINT] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::PROTO] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::INT32] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::INT64] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::UINT32] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::UINT64] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::FLOAT32] > 0);
        REQUIRE(token_counts[tooi::core::TokenType::FLOAT64] > 0);
    }
    
    SECTION("END_OF_FILE token is present") {
        INFO("Checking token: END_OF_FILE");
        REQUIRE(token_counts[tooi::core::TokenType::END_OF_FILE] > 0);
    }
}

// Test case 3: Whitespace and comment skipping
TEST_CASE("Scanner Skips Whitespace and Comments", "[scanner]") {
    TestErrorReporter error_reporter;
    // Source with spaces, tabs, newlines, and // style comments
    std::string source = "( // this is a line comment\n ) \t { /* another comment */\n }"; 
    tooi::core::Scanner scanner(source, error_reporter);

    INFO("Checking whitespace/comment skipping: Source='( //... ) \t { /*...*/ }', Expected tokens: (, ), {, }, EOF");
    std::vector<tooi::core::Token> tokens = scanner.scan_tokens();

    REQUIRE_FALSE(error_reporter.had_error());

    std::vector<tooi::core::TokenType> expected_types = {
        tooi::core::TokenType::LEFT_PAREN,
        tooi::core::TokenType::RIGHT_PAREN,
        tooi::core::TokenType::LEFT_BRACE,
        tooi::core::TokenType::RIGHT_BRACE,
        tooi::core::TokenType::END_OF_FILE
    };

    CAPTURE(tokens.size(), expected_types.size());
    REQUIRE(tokens.size() == expected_types.size());

    for (size_t i = 0; i < expected_types.size(); ++i) {
        CAPTURE(i, tokens[i].type, expected_types[i], tokens[i].lexeme);
        REQUIRE(tokens[i].type == expected_types[i]);
    }
}

TEST_CASE("Scanner Number Recognition", "[scanner]") {
    using namespace tooi::core;
    TestErrorReporter reporter;

    SECTION("Integer Literals (Stored as uint64_t magnitude)") {
        reporter.reset();
        Scanner scanner("123 0 -456 9", reporter);
        std::vector<Token> tokens = scanner.scan_tokens();

        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens.size() == 6); // NUMBER, NUMBER, MINUS, NUMBER, NUMBER, EOF

        INFO("Checking 123");
        REQUIRE(tokens[0].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens[0].literal));
        REQUIRE(std::get<uint64_t>(tokens[0].literal) == 123ULL);

        INFO("Checking 0");
        REQUIRE(tokens[1].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens[1].literal));
        REQUIRE(std::get<uint64_t>(tokens[1].literal) == 0ULL);

        INFO("Checking MINUS");
        REQUIRE(tokens[2].type == TokenType::MINUS);

        INFO("Checking 456");
        REQUIRE(tokens[3].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens[3].literal));
        REQUIRE(std::get<uint64_t>(tokens[3].literal) == 456ULL);

        INFO("Checking 9");
        REQUIRE(tokens[4].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens[4].literal));
        REQUIRE(std::get<uint64_t>(tokens[4].literal) == 9ULL);

        INFO("Checking EOF");
        REQUIRE(tokens[5].type == TokenType::END_OF_FILE);
    }

    SECTION("Floating-Point Literals (Stored as double)") {
        reporter.reset();
        Scanner scanner("123.45 0.0 -0.5 123f 456d 789.0f 1.0d", reporter);
        std::vector<Token> tokens = scanner.scan_tokens();

        REQUIRE_FALSE(reporter.had_error());
        // Expect: 123.45, 0.0, MINUS, 0.5, 123.0, 456.0, 789.0, 1.0, EOF
        REQUIRE(tokens.size() == 9);

        INFO("Checking 123.45"); REQUIRE(tokens[0].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[0].literal) == 123.45);
        INFO("Checking 0.0"); REQUIRE(tokens[1].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[1].literal) == 0.0);
        INFO("Checking MINUS"); REQUIRE(tokens[2].type == TokenType::MINUS);
        INFO("Checking 0.5"); REQUIRE(tokens[3].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[3].literal) == 0.5);
        INFO("Checking 123f -> double"); REQUIRE(tokens[4].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[4].literal) == 123.0);
        INFO("Checking 456d -> double"); REQUIRE(tokens[5].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[5].literal) == 456.0);
        INFO("Checking 789.0f -> double"); REQUIRE(tokens[6].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[6].literal) == 789.0);
        INFO("Checking 1.0d -> double"); REQUIRE(tokens[7].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[7].literal) == 1.0);
        INFO("Checking EOF"); REQUIRE(tokens[8].type == TokenType::END_OF_FILE);
    }

    SECTION("Numbers with Whitespace") {
        reporter.reset();
        Scanner scanner("  123 \n 45.67 \t ", reporter);
        std::vector<Token> tokens = scanner.scan_tokens();

        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens.size() == 3); // NUMBER(uint64), NUMBER(double), EOF
        REQUIRE(tokens[0].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[0].literal) == 123ULL);
        REQUIRE(tokens[1].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[1].literal) == 45.67);
        REQUIRE(tokens[2].type == TokenType::END_OF_FILE);
    }

    SECTION("Malformed Numbers - Form Errors") {
        // Multiple Decimals
        reporter.reset(); Scanner scanner_md("123.45.67", reporter); scanner_md.scan_tokens(); 
        REQUIRE(reporter.had_error()); 
        // Trailing Dot
        reporter.reset(); Scanner scanner_td("99.", reporter); scanner_td.scan_tokens(); 
        REQUIRE(reporter.had_error()); 
    }

    SECTION("Malformed Numbers - Invalid Suffix") {
        reporter.reset(); Scanner scanner_is("123foo", reporter); scanner_is.scan_tokens(); 
        REQUIRE(reporter.had_error()); 
    }

    SECTION("Malformed Numbers - Suffix Incompatibility") {
        reporter.reset(); Scanner scanner_si("1.23i32", reporter); scanner_si.scan_tokens(); 
        REQUIRE(reporter.had_error()); // Expect Scanner_InvalidSuffixForFloat
    }

    SECTION("Range Checks (Scanner only checks ULLONG/Double limits)") {
        // Test number > ULLONG_MAX (relies on stoull throwing)
        reporter.reset();
        std::string too_huge_number = "18446744073709551616"; // ULLONG_MAX + 1 approx
        Scanner scanner_too_huge(too_huge_number, reporter);
        scanner_too_huge.scan_tokens();
        REQUIRE(reporter.had_error()); // Expect stoull to throw OutOfRange

        // Test INT64_MAX + 1 magnitude (Stored as uint64_t, NO Scanner Error)
        reporter.reset();
        std::string i64_max_p1 = "9223372036854775808";
        Scanner scanner_i64(i64_max_p1, reporter);
        std::vector<Token> tokens_i64 = scanner_i64.scan_tokens();
        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens_i64.size() == 2);
        REQUIRE(tokens_i64[0].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens_i64[0].literal));
        REQUIRE(std::get<uint64_t>(tokens_i64[0].literal) == 9223372036854775808ULL);

        // Test INT32_MAX + 1 magnitude (Stored as uint64_t, NO Scanner Error)
        reporter.reset();
        std::string i32_max_p1 = "2147483648i32";
        Scanner scanner_i32(i32_max_p1, reporter);
        std::vector<Token> tokens_i32 = scanner_i32.scan_tokens();
        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens_i32.size() == 2);
        REQUIRE(tokens_i32[0].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens_i32[0].literal));
        REQUIRE(std::get<uint64_t>(tokens_i32[0].literal) == 2147483648ULL);

        // Test UINT32_MAX + 1 with u32 suffix (Scanner should NOT check this explicitly)
        reporter.reset();
        std::string u32_max_p1 = "4294967296u32"; // UINT32_MAX + 1
        Scanner scanner_u32(u32_max_p1, reporter);
        std::vector<Token> tokens_u32 = scanner_u32.scan_tokens();
        // FIX: Scanner no longer checks u32 range, expect NO error
        REQUIRE_FALSE(reporter.had_error()); 
        // Expect NUMBER(uint64_t), EOF
        REQUIRE(tokens_u32.size() == 2);
        INFO("Checking NUMBER token for UINT32_MAX + 1 magnitude");
        REQUIRE(tokens_u32[0].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens_u32[0].literal));
        REQUIRE(std::get<uint64_t>(tokens_u32[0].literal) == 4294967296ULL);
        REQUIRE(tokens_u32[1].type == TokenType::END_OF_FILE);
    }

    SECTION("Valid Suffixes") { // Replaces i32 Suffix Handling focus
        reporter.reset();
        const char* src = "1i 2u 3i32 4u32 5i64 6u64 7f 8d 9.0f 1.0d";
        Scanner scanner(src, reporter);
        std::vector<Token> tokens = scanner.scan_tokens();

        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens.size() == 11); // 10 numbers + EOF

        // Check integer forms stored as uint64_t
        REQUIRE(tokens[0].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[0].literal) == 1ULL); // 1i
        REQUIRE(tokens[1].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[1].literal) == 2ULL); // 2u
        REQUIRE(tokens[2].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[2].literal) == 3ULL); // 3i32
        REQUIRE(tokens[3].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[3].literal) == 4ULL); // 4u32
        REQUIRE(tokens[4].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[4].literal) == 5ULL); // 5i64
        REQUIRE(tokens[5].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<uint64_t>(tokens[5].literal) == 6ULL); // 6u64
        
        // Check float forms stored as double
        REQUIRE(tokens[6].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[6].literal) == 7.0); // 7f
        REQUIRE(tokens[7].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[7].literal) == 8.0); // 8d
        REQUIRE(tokens[8].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[8].literal) == 9.0); // 9.0f
        REQUIRE(tokens[9].type == TokenType::NUMBER_LITERAL); REQUIRE(std::get<double>(tokens[9].literal) == 1.0); // 1.0d

        REQUIRE(tokens[10].type == TokenType::END_OF_FILE);
    }

    SECTION("Handling INT_MIN Input String") {
        reporter.reset();
        Scanner scanner_i32_min("-2147483648i32", reporter);
        std::vector<Token> tokens_i32_min = scanner_i32_min.scan_tokens();
        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens_i32_min.size() == 3);
        REQUIRE(tokens_i32_min[0].type == TokenType::MINUS);
        REQUIRE(tokens_i32_min[1].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens_i32_min[1].literal));
        REQUIRE(std::get<uint64_t>(tokens_i32_min[1].literal) == 2147483648ULL);
        REQUIRE(tokens_i32_min[2].type == TokenType::END_OF_FILE);

        reporter.reset();
        Scanner scanner_i64_min("-9223372036854775808", reporter);
        std::vector<Token> tokens_i64_min = scanner_i64_min.scan_tokens();
        REQUIRE_FALSE(reporter.had_error());
        REQUIRE(tokens_i64_min.size() == 3);
        REQUIRE(tokens_i64_min[0].type == TokenType::MINUS);
        REQUIRE(tokens_i64_min[1].type == TokenType::NUMBER_LITERAL);
        REQUIRE(std::holds_alternative<uint64_t>(tokens_i64_min[1].literal));
        REQUIRE(std::get<uint64_t>(tokens_i64_min[1].literal) == 9223372036854775808ULL);
        REQUIRE(tokens_i64_min[2].type == TokenType::END_OF_FILE);
    }

    // TODO: Add tests for hex, binary, octal if/when supported
    // TODO: Add tests for scientific notation (e.g., 1.23e4) if/when supported
}

TEST_CASE("Scanner Example Program", "[scanner]") {
    TestErrorReporter error_reporter;
    std::string source = R"(
add io;

set main @ {
    io.@print_line("Hello, Tooi!");
};
)";
    
    tooi::core::Scanner scanner(source, error_reporter);
    std::vector<tooi::core::Token> tokens = scanner.scan_tokens();
    
    INFO("Checking hello.tooi example program scanning");
    REQUIRE_FALSE(error_reporter.had_error());
    
    // Expected token sequence
    std::vector<tooi::core::TokenType> expected_types = {
        tooi::core::TokenType::ADD,           // add
        tooi::core::TokenType::IDENTIFIER_LITERAL,  // io
        tooi::core::TokenType::SEMICOLON,     // ;
        tooi::core::TokenType::SET,           // set
        tooi::core::TokenType::IDENTIFIER_LITERAL,  // main
        tooi::core::TokenType::AT,            // @
        tooi::core::TokenType::LEFT_BRACE,    // {
        tooi::core::TokenType::IDENTIFIER_LITERAL,  // io
        tooi::core::TokenType::DOT,           // .
        tooi::core::TokenType::AT,            // @
        tooi::core::TokenType::IDENTIFIER_LITERAL,  // print_line
        tooi::core::TokenType::LEFT_PAREN,    // (
        tooi::core::TokenType::STRING_LITERAL, // "Hello, Tooi!"
        tooi::core::TokenType::RIGHT_PAREN,   // )
        tooi::core::TokenType::SEMICOLON,     // ;
        tooi::core::TokenType::RIGHT_BRACE,   // }
        tooi::core::TokenType::SEMICOLON,     // ;
        tooi::core::TokenType::END_OF_FILE    // EOF
    };
    
    CAPTURE(tokens.size(), expected_types.size());
    REQUIRE(tokens.size() == expected_types.size());
    
    // Check each token
    for (size_t i = 0; i < expected_types.size(); ++i) {
        CAPTURE(i, tokens[i].type, expected_types[i], tokens[i].lexeme);
        REQUIRE(tokens[i].type == expected_types[i]);
        
        // Additional checks for specific tokens
        if (tokens[i].type == tooi::core::TokenType::STRING_LITERAL) {
            REQUIRE(tokens[i].lexeme == "\"Hello, Tooi!\"");
        } else if (tokens[i].type == tooi::core::TokenType::IDENTIFIER_LITERAL) {
            if (i == 1) REQUIRE(tokens[i].lexeme == "io");
            else if (i == 4) REQUIRE(tokens[i].lexeme == "main");
            else if (i == 8) REQUIRE(tokens[i].lexeme == "io");
            else if (i == 10) REQUIRE(tokens[i].lexeme == "print_line");
        }
    }
}
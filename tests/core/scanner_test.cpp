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
    void report_at(int line, int column, int length, const std::string& source_line, tooi::core::ErrorCode code, const std::string& message) {
        // You could store errors in a vector for detailed inspection if needed
        // std::cerr << "Test Error: " << message << " at " << line << ":" << column << std::endl;
        reported_errors_.push_back({line, column, message});
        had_error_ = true; // Set flag
    }
    void report_general(tooi::core::ErrorCode code, const std::string& message) {
        // std::cerr << "Test General Error: " << message << std::endl;
        reported_errors_.push_back({-1, -1, message}); // Use special values for general errors
        had_error_ = true;
    }
    bool had_error() const { return had_error_; }
    void reset() {
         had_error_ = false;
         reported_errors_.clear();
    }

    // Helper to check if specific errors were reported (optional)
    const auto& get_errors() const { return reported_errors_; }

private:
    struct ErrorInfo {
        int line;
        int column;
        std::string message;
    };
    bool had_error_ = false;
    std::vector<ErrorInfo> reported_errors_;
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
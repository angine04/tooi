#pragma once

#include <string>
#include <vector>
#include "tooi/core/token.h" // Include the Token definition
#include "tooi/core/error_reporter.h" // Include ErrorReporter

namespace tooi {
namespace core {

class Scanner {
public:
    /**
     * @brief Constructs a Scanner instance.
     * @param source The source code string to scan.
     * @param error_reporter Reference to the error reporter to use.
     */
    Scanner(std::string source, ErrorReporter& error_reporter);

    /**
     * @brief Scans the source code and returns a list of tokens.
     *
     * Processes the entire source string provided during construction.
     * It handles recognizing lexemes for operators, literals (numbers, strings,
     * identifiers), keywords, and whitespace/comments.
     *
     * @return A vector of Token objects representing the scanned source code.
     *         Includes an END_OF_FILE token at the end.
     *         May include ERROR tokens if lexical errors are encountered.
     */
    std::vector<Token> scan_tokens();

private:
    std::string source_; // The source code being scanned
    std::vector<Token> tokens_; // List of tokens generated
    int start_ = 0;   // Start index of the current lexeme being scanned
    int current_ = 0; // Current index scanning through the source
    int line_ = 1;    // Current line number
    int line_start_ = 0; // Index of the start of the current line
    ErrorReporter& error_reporter_; // Store reference to the reporter

    // Helper methods for scanning logic
    bool is_at_end() const;
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, const TokenLiteral& literal);
    bool match(char expected);
    char peek() const;
    char peek_next() const;

    void scan_token(); // Main dispatch method for scanning one token
    void skip_whitespace_and_comments();
    void scan_interpolated_string(char delimiter);
    void scan_raw_string();
    void scan_number();
    void scan_identifier();

    // Helper to create an error token (could also directly add to tokens_)
    // Token make_error_token(const std::string& message) const;

    void report_error_here(int length, const std::string& message); // Simplified reporting
}; 

} // namespace core
} // namespace tooi 
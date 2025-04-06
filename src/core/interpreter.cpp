/**
 * @file interpreter.cpp
 * @brief Implementation of the Interpreter class for executing Tooi scripts.
 */
#include "tooi/core/interpreter.h"

#include <fstream> // Still potentially useful
#include <iostream>
#include <istream>
#include <sstream> // Needed to read stream into string
#include <string>
#include <vector>

#include "tooi/core/scanner.h" // Include the Scanner header
#include "tooi/core/token.h"   // Include the Token header
#include "tooi/core/error_reporter.h"

namespace tooi {
namespace core {

/**
 * @brief Executes Tooi code read from the given input stream.
 *
 * Reads the entire stream content, tokenizes it using the Scanner,
 * and currently just prints the resulting tokens.
 * Modifies the interpreter's state (e.g., increments execution count).
 *
 * @param input_stream The input stream providing the Tooi code.
 * @return True if the code stream was read and scanned without fatal I/O errors.
 *         Scanner errors (lexical errors) might result in ERROR tokens but won't return false here.
 */
bool Interpreter::run(std::istream& input_stream) {
    error_reporter_.reset(); // Reset error state for this run
    execution_count_++; // Increment state counter
    if (verbose_) {
        std::cout << "[Interpreter::run call #" << execution_count_ << "] Processing stream..." << std::endl;
    }

    // 1. Read the entire stream into a string
    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    std::string source = buffer.str();

    // Check for stream errors *after* reading
    if (input_stream.bad() || (input_stream.fail() && !input_stream.eof())) {
         // Report stream error using the reporter's new method
         // Provide placeholder values as context isn't readily available here
         error_reporter_.report_at(1, 1, 1, "", "Error reading input stream.");
         return false; // Still return false on stream read error
    }
    if (input_stream.eof()) {
         input_stream.clear();
    }

    // 2. Scan the source string into tokens
    Scanner scanner(std::move(source), error_reporter_);
    std::vector<Token> tokens = scanner.scan_tokens();

    // 3. Print the tokens only if verbose mode is enabled
    if (verbose_) {
        std::cout << "  Scanned " << tokens.size() << " tokens:" << std::endl;
        for (const auto& token : tokens) {
            std::cout << "    " << token.to_string() << std::endl;
        }
    }

    // After scanning, check if the scanner reported errors
    if (error_reporter_.had_error()) {
        // We might want to stop here, or continue to parsing to find more errors
        // For now, let's return true but the caller can check had_error()
    }

    // TODO: Placeholder for actual processing (Parsing, Evaluation):
    // Parser parser(tokens, error_reporter_);
    // auto statements = parser.parse();
    // if (!parser.has_errors()) {
    //    evaluate(statements);
    // }

    // Return true if no FATAL errors occurred (like stream read error)
    // The caller should check interpreter.had_error() for lexical/parse/etc. errors
    return true;
}

Interpreter::Interpreter(bool verbose)
    : verbose_(verbose) {}

bool Interpreter::had_error() const {
    return error_reporter_.had_error();
}

} // namespace core
} // namespace tooi
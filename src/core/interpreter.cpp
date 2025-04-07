/**
 * @file interpreter.cpp
 * @brief Implementation of the Interpreter class for executing Tooi scripts.
 */
#include "tooi/core/interpreter.h"

#include <iostream>
#include <istream>
#include <sstream> // Needed to read stream into string
#include <string>
#include <vector>

#include "tooi/core/scanner.h" // Include the Scanner header
#include "tooi/core/token.h"   // Include the Token header
#include "tooi/core/error_reporter.h"
#include "tooi/cli/colors.h" // Include colors
#include "tooi/core/error_info.h"

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
    using namespace tooi::cli::colors; // Add using declaration
    error_reporter_.reset(); // Reset error state for this run
    execution_count_++; // Increment state counter
    if (verbose_) {
        // Use Blue for verbose status messages from interpreter
        std::cout << BOLD_BLUE << "[Interpreter::run call #" << execution_count_ << "] Processing stream..." << RESET << std::endl;
    }

    // 1. Read the entire stream into a string
    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    std::string source = buffer.str();

    // Check for stream errors *after* reading
    if (input_stream.bad() || (input_stream.fail() && !input_stream.eof())) {
         // Report stream error using the reporter's new method
         // Provide placeholder values as context isn't readily available here
         error_reporter_.report_at(1, 1, 1, "", ErrorCode::Interpreter_StreamReadError);
         return false; // Still return false on stream read error
    }
    if (input_stream.eof()) {
         input_stream.clear();
    }

    // 2. Scan the source string into tokens
    Scanner scanner(std::move(source), error_reporter_);
    // source has been moved, do not use it again
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
        // Use Bold Red for this halting message
        std::cerr << BOLD_RED << "Fatal: Halting due to lexical errors." << RESET << std::endl;
        return true;
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
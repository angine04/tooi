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
    execution_count_++; // Increment state counter
    std::cout << "[Interpreter::run call #" << execution_count_ << "] Processing stream..." << std::endl;

    // 1. Read the entire stream into a string
    std::stringstream buffer;
    buffer << input_stream.rdbuf(); // Read everything from input_stream into buffer
    std::string source = buffer.str();

    // Check for stream errors *after* reading
    if (input_stream.bad() || (input_stream.fail() && !input_stream.eof())) {
         std::cerr << "Error: Failed while reading from input stream." << std::endl;
         // Optionally clear flags if needed: input_stream.clear();
         return false; // Indicate stream read error before scanning
    }
    // Clear EOF flag if necessary (though usually not needed after reading to string)
    if (input_stream.eof()) {
         input_stream.clear();
    }

    // 2. Scan the source string into tokens
    Scanner scanner(std::move(source)); // Use std::move if source won't be needed again
    std::vector<Token> tokens = scanner.scan_tokens();

    std::cout << "  Scanned " << tokens.size() << " tokens:" << std::endl;

    // 3. Print the tokens (placeholder for actual execution)
    for (const auto& token : tokens) {
        std::cout << "    " << token.to_string() << std::endl;
        // TODO: Future: Check for TokenType::ERROR and report/handle properly
    }

    // TODO: Placeholder for actual processing (Parsing, Evaluation):
    // Parser parser(tokens);
    // auto statements = parser.parse();
    // if (!parser.has_errors()) {
    //    evaluate(statements);
    // }

    return true; // Indicate successful processing of the stream content (for now)
}

} // namespace core
} // namespace tooi
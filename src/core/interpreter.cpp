/**
 * @file interpreter.cpp
 * @brief Implementation of the Interpreter class for executing Tooi scripts.
 */
#include "tooi/core/interpreter.h"
#include <fstream>
#include <iostream>
#include <sstream> // Include for potential future use with string streams
#include <string>

namespace tooi {
namespace core {

/**
 * @brief Executes a Tooi script read from a specified file.
 *
 * Opens the file, reads it line by line (currently just printing them),
 * and reports any errors encountered during file opening or reading.
 *
 * @param filename The path to the script file.
 * @return True if the file was opened and read successfully (even if the script has errors),
 *         false if the file could not be opened or a read error occurred.
 */
bool Interpreter::run_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"" << std::endl;
        return false; // Indicate failure to open
    }

    std::string line;
    // Basic line-by-line processing for now
    while (std::getline(file, line)) {
        // Placeholder for actual processing:
        // 1. Lexing: Tokenize the line
        // 2. Parsing: Build AST (might span multiple lines)
        // 3. Evaluation: Execute
        std::cout << "Executing line: " << line << std::endl; // Simple echo for now
    }

    // Check for I/O errors after the loop (e.g., disk error during read)
    if (file.bad()) {
        std::cerr << "Error: Failed while reading file \"" << filename << "\"" << std::endl;
        // file object is automatically closed when it goes out of scope (RAII)
        return false; // Indicate read error
    }

    // Check if the loop ended due to reaching EOF (good) vs other errors
    // file.eof() is true if EOF was reached. file.fail() might be true if getline failed not due to EOF.
    // For this simple case, reaching here without file.bad() is considered success.

    // file object is automatically closed when it goes out of scope (RAII)
    return true; // Indicate successful completion of file processing
}

} // namespace core
} // namespace tooi
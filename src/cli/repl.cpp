/**
 * @file repl.cpp
 * @brief Implementation of the Repl class for the Read-Eval-Print Loop.
 */
#include "tooi/cli/repl.h"
#include <iostream>
#include <string>
#include <sstream> // Needed for stringstream in Repl::run modification
#include <vector> // To buffer lines in Repl::run modification? Maybe remove later if unused.
#include <cstddef> // For size_t

// Moved run_from_file implementation to its own file.
// Removed includes related to run_from_file: <fstream>, "tooi/core/interpreter.h"
#include "tooi/core/interpreter.h" // Interpreter is still needed for Repl::run

namespace tooi {
namespace cli {

// Constructor implementation
Repl::Repl(bool verbose)
    : verbose_(verbose) {}

/**
 * @brief Runs the Read-Eval-Print Loop (REPL).
 *
 * Displays a welcome message and prompt. Reads user input, potentially across
 * multiple lines, until an empty line is entered. Executes the accumulated code
 * block using a persistent Interpreter instance. Exits on "@exit;" or EOF.
 */
void Repl::run() {
    core::Interpreter interpreter(verbose_); // Pass verbose flag to interpreter
    std::string current_block;     // Buffer for the current multi-line block
    std::string line;
    bool need_more_input = false; // Flag to control prompt display

    std::cout << "Welcome to Tooi REPL! Enter code, finish block with empty line. Type '@exit;' or Ctrl+D to quit." << std::endl;

    while (true) {
        // Display appropriate prompt
        std::cout << (need_more_input ? "  ... " : "tooi> ");

        // Read a whole line from standard input
        if (!std::getline(std::cin, line)) {
            // Handle EOF (End Of File), typically Ctrl+D in Unix-like systems
            // If there's content in the buffer, try to execute it before exiting
            if (!current_block.empty()) {
                std::cout << std::endl; // Newline after last input
                std::stringstream ss(current_block);
                interpreter.run(ss); // Verbose output handled inside run
            }
            std::cout << std::endl; // Ensure cursor moves to the next line on exit
            break;
        }

        // Check for the exit command *only* if the buffer is empty (start of new command)
        if (current_block.empty() && line == "@exit;") {
            break;
        }

        // Check if the line is empty to signal end of block
        if (line.empty()) {
            if (!current_block.empty()) {
                // Execute the accumulated block
                std::stringstream ss(current_block);
                if (!interpreter.run(ss)) { // Verbose output handled inside run
                   // Error messages might depend on verbose flag within run()
                   std::cerr << "Error during execution. Block cleared." << std::endl;
                }
                current_block.clear(); // Clear buffer for the next block
                need_more_input = false; // Reset prompt
            } else {
                // Empty line when buffer is empty, just show primary prompt again
                need_more_input = false;
            }
        } else {
            // Non-empty line
            current_block += line + '\n';

            bool is_first_line = !need_more_input;
            bool ends_with_semicolon = false;

            // Check if the original line ends with a semicolon (ignoring trailing whitespace)
            size_t endpos = line.find_last_not_of(" \t\n\r");
            if (endpos != std::string::npos && line[endpos] == ';') {
                ends_with_semicolon = true;
            }

            // If it's the first line of a block AND it ends with a semicolon, execute immediately
            if (is_first_line && ends_with_semicolon) {
                std::stringstream ss(current_block);
                if (!interpreter.run(ss)) {
                   std::cerr << "Error during execution. Block cleared." << std::endl;
                }
                current_block.clear(); // Clear buffer after execution
                need_more_input = false; // Ready for next primary prompt
            } else {
                // Otherwise, continue accumulating the block
                need_more_input = true;
            }
        }
    }
    std::cout << "Exiting Tooi REPL." << std::endl;
}

// Removed run_from_file implementation from here.

} // namespace cli
} // namespace tooi
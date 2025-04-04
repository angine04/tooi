/**
 * @file repl.cpp
 * @brief Implementation of the Repl class for the Read-Eval-Print Loop.
 */
#include "tooi/cli/repl.h"
#include <iostream>
#include <string>
#include <sstream> // Needed for stringstream in Repl::run modification
#include <vector> // To buffer lines in Repl::run modification? Maybe remove later if unused.

// Moved run_from_file implementation to its own file.
// Removed includes related to run_from_file: <fstream>, "tooi/core/interpreter.h"
#include "tooi/core/interpreter.h" // Interpreter is still needed for Repl::run

namespace tooi {
namespace cli {

/**
 * @brief Runs the Read-Eval-Print Loop (REPL).
 *
 * Displays a welcome message and prompt. Reads user input, potentially across
 * multiple lines, until an empty line is entered. Executes the accumulated code
 * block using a persistent Interpreter instance. Exits on "@exit;" or EOF.
 */
void Repl::run() {
    core::Interpreter interpreter; // Create a persistent interpreter instance
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
                interpreter.run(ss);
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
                if (!interpreter.run(ss)) {
                   // Interpreter reported an error (e.g., stream read error or fatal parse/exec error)
                   // Error message should have been printed by interpreter.run
                   std::cerr << "Error during execution. Block cleared." << std::endl;
                }
                current_block.clear(); // Clear buffer for the next block
                need_more_input = false; // Reset prompt
            } else {
                // Empty line when buffer is empty, just show primary prompt again
                need_more_input = false;
            }
        } else {
            // Non-empty line, append to the buffer
            current_block += line + '\n';
            need_more_input = true; // Indicate we need more input (show '...')
        }
    }
    std::cout << "Exiting Tooi REPL." << std::endl;
}

// Removed run_from_file implementation from here.

} // namespace cli
} // namespace tooi
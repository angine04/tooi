/**
 * @file repl.cpp
 * @brief Implementation of the Repl class for the Read-Eval-Print Loop.
 */
#include "tooi/cli/repl.h"
#include <iostream>
#include <string>
// #include <vector> // Keep if needed later, otherwise remove

namespace tooi {
namespace cli {

/**
 * @brief Runs the Read-Eval-Print Loop (REPL).
 *
 * Displays a welcome message and prompt, reads user input line by line,
 * processes the input (currently just echoes it), and exits when the user
 * types "exit" or provides an EOF signal (like Ctrl+D).
 */
void Repl::run() {
    std::string line;
    std::cout << "Welcome to Tooi REPL! Type 'exit' or press Ctrl+D to quit." << std::endl;

    while (true) {
        std::cout << "tooi> ";
        // Read a whole line from standard input
        if (!std::getline(std::cin, line)) {
            // Handle EOF (End Of File), typically Ctrl+D in Unix-like systems
            std::cout << std::endl; // Ensure cursor moves to the next line on exit
            break;
        }

        // Trim leading/trailing whitespace? (Optional)

        // Check for the exit command
        if (line == "exit") {
            break;
        }

        // Placeholder for actual processing:
        // 1. Lexing: Tokenize the input line
        // 2. Parsing: Build an Abstract Syntax Tree (AST)
        // 3. Evaluation: Execute the AST
        // 4. Printing: Display the result
        std::cout << "Echo: " << line << std::endl; // Simple echo for now

    }
    std::cout << "Exiting Tooi REPL." << std::endl;
}

} // namespace cli
} // namespace tooi 
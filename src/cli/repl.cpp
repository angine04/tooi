/**
 * @file repl.cpp
 * @brief Implementation of the Repl class for the Read-Eval-Print Loop.
 */
#include "tooi/cli/repl.h"

#include <cerrno>    // For errno
#include <cstddef>   // For size_t
#include <cstdio>    // For perror()
#include <cstdlib>   // For free()
#include <iostream>  // For std::cout, std::cerr, std::endl
#include <sstream>   // For std::stringstream
#include <string>    // For std::string

#include "linenoise.h"              // For line editing, history, and input handling
#include "tooi/core/interpreter.h"  // For the core interpreter logic

namespace tooi {
namespace cli {

// Constructor implementation
Repl::Repl(bool verbose) : verbose_(verbose) {}

/**
 * @brief Runs the Read-Eval-Print Loop (REPL) using the linenoise library.
 *
 * This function provides an interactive command line for the Tooi interpreter.
 * It utilizes linenoise for features like line editing (arrow keys), history
 * navigation (up/down arrows), and basic tab completion hooks (though completion
 * callback is not set here).
 *
 * It handles multi-line input by accumulating lines until an empty line is entered.
 * It also supports immediate execution for single lines ending with a semicolon.
 * Ctrl+C interrupts the current input line, while Ctrl+D exits the REPL.
 */
void Repl::run() {
    core::Interpreter interpreter(verbose_);  // Create the interpreter instance
    std::string current_block;                // Buffer to accumulate multi-line input
    bool need_more_input = false;  // Flag to determine which prompt to show ('>' or '...')
    char *line_c_str = nullptr;    // Raw C-string buffer returned by linenoise
    std::string history_file = ".tooi_history";  // File to store command history
    int consecutive_interrupts = 0;              // Counter for consecutive Ctrl+C presses

    // --- Linenoise Configuration ---
    linenoiseSetMultiLine(1);                    // Enable multi-line mode (allows Shift+Enter etc.)
    linenoiseHistoryLoad(history_file.c_str());  // Load history from the file

    // --- Welcome Message ---
    std::cout << "Welcome to Tooi REPL! Use Up/Down for history. "
              << "Finish block with empty line. Ctrl+C to interrupt, "
              << "Ctrl+D or @exit; to quit." << std::endl;

    // --- Main REPL Loop ---
    while (true) {
        // Determine the prompt based on whether we expect more lines for the current block
        const char *prompt = (need_more_input ? "  ... " : "tooi> ");

        // Reset errno before calling linenoise, as linenoise uses it to signal Ctrl+C/Ctrl+D
        errno = 0;
        // Call linenoise to get a line of input. This handles line editing, history, etc.
        // It blocks until the user presses Enter, Ctrl+C, or Ctrl+D.
        line_c_str = linenoise(prompt);

        // --- Input Handling ---
        if (line_c_str == nullptr) {
            // linenoise returns nullptr in several cases (Ctrl+C, Ctrl+D, errors).
            // We need to check errno to differentiate.
            if (errno == EAGAIN) {
                // EAGAIN signals that Ctrl+C was pressed (interrupt) based on linenoise.c source.
                consecutive_interrupts++;
                std::cout << "KeyboardInterrupt" << std::endl;
                current_block.clear();    // Discard any partially entered block
                need_more_input = false;  // Go back to the primary prompt
                if (consecutive_interrupts >= 3) {
                    // Provide a hint if the user presses Ctrl+C repeatedly
                    std::cout << "(Hint: Use @exit; or Ctrl+D to exit)" << std::endl;
                }
                continue;  // Go back to the start of the loop to show the prompt again
            } else if (errno == ENOENT) {
                // ENOENT signals that Ctrl+D was pressed on an empty line (EOF) based on
                // linenoise.c source.
                std::cout << std::endl;  // Print a newline for cleaner exit
                break;                   // Exit the REPL loop
            } else {
                // Some other error occurred within linenoise or during read.
                if (errno != 0) {
                    perror("linenoise error");  // Print the system error message
                }
                std::cout << std::endl;
                break;  // Exit the REPL on other errors too
            }
        }

        // --- We received a valid line ---
        std::string line(line_c_str);  // Convert the C-string to std::string
        free(line_c_str);              // IMPORTANT: Free the memory allocated by linenoise

        // Reset the interrupt counter since we received valid input
        consecutive_interrupts = 0;

        // Check for our custom exit command (only effective at the primary prompt)
        if (current_block.empty() && line == "@exit;") {
            break;  // Exit the REPL loop
        }

        // --- Multi-line and Submission Logic ---

        // Case 1: Empty line submitted - signifies end of a multi-line block
        if (line.empty() && !current_block.empty()) {
            // Execute the accumulated block
            std::stringstream ss(current_block);
            interpreter.run(ss);  // Errors are handled by the interpreter's reporter
            // Add the completed block to history for up-arrow recall
            current_block.clear();                       // Clear the buffer for the next input
            need_more_input = false;                     // Reset to show primary prompt
            continue;                                    // Get next input
        }
        // Case 2: Empty line submitted, but no block was being entered - do nothing
        else if (line.empty() && current_block.empty()) {
            need_more_input = false;  // Ensure primary prompt
            continue;                 // Get next input
        }

        // Case 3: Non-empty line submitted - append and check for immediate execution
        current_block += line + '\n';
        // Add the individual line to history for better line-editing recall
        linenoiseHistoryAdd(line.c_str());
        linenoiseHistorySave(history_file.c_str());  // Persist history

        bool is_first_line = !need_more_input;  // Was this the first line after the primary prompt?
        bool ends_with_semicolon = false;

        // Check if the entered line ends with ';' (ignoring trailing whitespace)
        size_t endpos = line.find_last_not_of(" \t\n\r");
        if (endpos != std::string::npos && line[endpos] == ';') {
            ends_with_semicolon = true;
        }

        // Special case: If it's the first line AND ends with ';', execute immediately
        if (is_first_line && ends_with_semicolon) {
            std::stringstream ss(current_block);
            interpreter.run(ss);                         // Errors handled by reporter
            linenoiseHistorySave(history_file.c_str());  // Persist history
            current_block.clear();                       // Clear buffer
            need_more_input = false;                     // Reset to primary prompt
        } else {
            // Otherwise, it's part of a multi-line block (or a single line not ending in ';')
            need_more_input = true;  // Set flag to show "..." prompt next time
        }
    }  // End while(true) loop

    // --- REPL Exit ---
    std::cout << "Exiting Tooi REPL." << std::endl;
    // No explicit signal handler restoration needed as we didn't override linenoise's internal
    // handling.
}

}  // namespace cli
}  // namespace tooi
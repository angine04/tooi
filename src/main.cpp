/**
 * @file main.cpp
 * @brief Main entry point for the Tooi interpreter application.
 */
#include <iostream>
#include <string>
#include <vector>

// Include Catch2 header only if tests are enabled
#ifdef TOOI_TESTS_ENABLED
#define CATCH_CONFIG_RUNNER // Define runner BEFORE including header
#include "catch2.hpp"
#endif

// Assuming build system handles include paths correctly:
#include "tooi/cli/args_parser.h"
#include "tooi/cli/repl.h"
#include "tooi/cli/run_from_file.h"
#include "tooi/cli/colors.h" // Include colors
// #include "tooi/core/interpreter.h" // No longer directly needed here

#ifdef _WIN32
// ... windows enable virtual term ...
#endif

/**
 * @brief The main function, entry point of the program.
 *
 * Parses command-line arguments and runs the interpreter in the appropriate mode
 * (REPL, file execution, help, version) or reports errors.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return 0 on successful execution, 1 on error (argument error or file processing error).
 */
int main(int argc, char* argv[]) {

    using namespace tooi::cli::colors;

#ifdef TOOI_TESTS_ENABLED
    // Check if the first argument is --run-tests
    if (argc > 1 && std::string(argv[1]) == "--run-tests") {
        std::cout << BOLD_GREEN << "Running tests..." << RESET << std::endl;
        // Create a new vector to store arguments for Catch2
        std::vector<char*> catch_argv;
        catch_argv.push_back(argv[0]); // Program name
        for (int i = 2; i < argc; ++i) { // Start from argv[2] if any test-specific args exist
            catch_argv.push_back(argv[i]);
        }
        int result = Catch::Session().run(catch_argv.size(), catch_argv.data());
        return result;
    }
#endif

#ifndef TOOI_TESTS_ENABLED
     if (argc > 1 && std::string(argv[1]) == "--run-tests"){
        std::cerr << BOLD_RED << "Error: Tests are not enabled. Please reconfigure the project with the -DTOOI_ENABLE_TESTS=ON option." << RESET << std::endl;
        return 1;
     }
#endif

    // Use using declarations for clarity within the main function scope
    using tooi::cli::ArgsParser;
    using tooi::cli::Repl;
    using tooi::cli::RunMode;
    // using tooi::core::Interpreter; // No longer directly needed here
    // Explicitly using the function from its namespace
    // using tooi::cli::run_from_file; // Alternative: Just qualify the call

    ArgsParser args_parser;
    args_parser.parse(argc, argv);

    RunMode mode = args_parser.get_mode();
    bool verbose = args_parser.is_verbose(); // Get verbose flag
    int exit_code = 0;  // Default to success

    switch (mode) {
        case RunMode::HELP:
            args_parser.show_help(argv[0]);
            break;
        case RunMode::VERSION:
            args_parser.show_version();
            break;
        case RunMode::REPL: {
            if (verbose) {
                // Use Magenta for verbose status
                std::cout << BOLD_MAGENTA << "Starting REPL in verbose mode..." << RESET << std::endl;
            }
            Repl repl(verbose); // Pass flag to Repl constructor
            repl.run();
        } break;
        case RunMode::FILE: {
            if (verbose) {
                // Use Magenta for verbose status
                std::cout << BOLD_MAGENTA << "Running file in verbose mode: " << args_parser.get_filename() << RESET << std::endl;
            }
            // Pass flag to run_from_file
            if (!tooi::cli::run_from_file(args_parser.get_filename(), verbose)) {
                // run_from_file now returns false if errors occurred
                exit_code = 1;
            }
        } break;
        case RunMode::ERROR: {
            // Use Bold Red for argument errors reported by main
            std::cerr << BOLD_RED << "Error: Invalid arguments." << RESET << std::endl;
            args_parser.show_help(argv[0]);
            exit_code = 1;  // Indicate argument error
        } break;
    }

    return exit_code;
}
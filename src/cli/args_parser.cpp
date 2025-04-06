/**
 * @file args_parser.cpp
 * @brief Implementation of the ArgsParser class for command-line argument parsing.
 */
#include "tooi/cli/args_parser.h"
#include <iostream>
#include <string>
#include <vector> // For processing arguments

namespace {
// ANSI Color Codes (simple version)
// Define separately here or move to a common header later
const char* const COLOR_CYAN_BOLD = "\x1B[1;36m";
const char* const COLOR_YELLOW = "\x1B[0;33m"; // Normal yellow
const char* const COLOR_RESET = "\x1B[0m";
} // anonymous namespace

namespace tooi {
namespace cli {

/**
 * Parses the command-line arguments to determine the run mode, filename,
 * and flags like verbose mode.
 */
void ArgsParser::parse(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc); // Easier to work with strings
    std::string potential_filename;
    mode_ = RunMode::REPL; // Default assumption

    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            mode_ = RunMode::HELP;
            return; // Help overrides everything else
        } else if (arg == "-v" || arg == "--version") { // Treat -v as version for now
            mode_ = RunMode::VERSION;
            return; // Version overrides everything else
        } else if (arg == "--verbose") {
            verbose_ = true;
            // Continue parsing other args
        } else if (arg.rfind("-", 0) == 0) {
             // Unknown option
             mode_ = RunMode::ERROR;
             error_message_ = "Unknown option: " + arg;
             return;
        } else {
            // Assume it's a filename. Only allow one.
            if (!potential_filename.empty()) {
                mode_ = RunMode::ERROR;
                error_message_ = "Multiple filenames provided.";
                return;
            }
            potential_filename = arg;
        }
    }

    // Post-processing logic
    if (mode_ != RunMode::HELP && mode_ != RunMode::VERSION && mode_ != RunMode::ERROR) {
        if (!potential_filename.empty()) {
             mode_ = RunMode::FILE;
             filename_ = potential_filename;
        } else {
             // No filename given, stays in REPL mode (unless verbose made it error above)
             mode_ = RunMode::REPL;
        }
    }
    // Note: This logic implies --verbose can be used with REPL or FILE mode.
    // If --verbose is the *only* argument, it will enter REPL in verbose mode.
}

/**
 * Returns the run mode determined by the parse() method.
 */
RunMode ArgsParser::get_mode() const {
    return mode_;
}

/**
 * Returns the filename specified on the command line.
 * Returns an empty string if no filename was provided or mode is not FILE.
 */
const std::string& ArgsParser::get_filename() const {
    return filename_;
}

/**
 * Checks if verbose mode was requested.
 */
bool ArgsParser::is_verbose() const {
    return verbose_;
}

/**
 * Prints the standard help message to standard error (with color).
 */
void ArgsParser::show_help(const char* program_name) const {
    std::cerr << COLOR_CYAN_BOLD << "Usage:" << COLOR_RESET << " " << program_name << " [options] [file]\n";
    std::cerr << COLOR_CYAN_BOLD << "\nOptions:\n" << COLOR_RESET;
    std::cerr << "  " << COLOR_YELLOW << "-h, --help" << COLOR_RESET << "     Show this help message and exit\n";
    std::cerr << "  " << COLOR_YELLOW << "-v, --version" << COLOR_RESET << "  Show version information and exit\n";
    std::cerr << "      " << COLOR_YELLOW << "--verbose" << COLOR_RESET << "  Enable verbose output during execution\n";
    std::cerr << COLOR_CYAN_BOLD << "\nArguments:\n" << COLOR_RESET;
    std::cerr << "  " << COLOR_YELLOW << "file" << COLOR_RESET << "           Execute the script from the specified file\n";
    std::cerr << "\nIf no file is provided, tooi starts in REPL mode.\n";
}

/**
 * Prints the version information to standard output.
 */
void ArgsParser::show_version() const {
    // TODO: Replace with a mechanism to get version from build system (e.g., CMake variable)
    std::cout << "tooi interpreter version 0.0.1 (pre-alpha)" << std::endl;
}

}  // namespace cli
}  // namespace tooi
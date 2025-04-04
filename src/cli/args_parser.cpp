/**
 * @file args_parser.cpp
 * @brief Implementation of the ArgsParser class for command-line argument parsing.
 */
#include "tooi/cli/args_parser.h"
#include <iostream>
#include <string>

namespace tooi {
namespace cli {

/**
 * Parses the command-line arguments to determine the run mode and filename.
 * This is a simple implementation and might need refinement for more complex cases.
 */
void ArgsParser::parse(int argc, char* argv[]) {
    if (argc == 1) {
        // No arguments, default to REPL mode
        mode_ = RunMode::REPL;
        return;
    }

    // Convert first argument to std::string for easier comparison
    std::string arg1 = argv[1];

    if (arg1 == "-h" || arg1 == "--help") {
        mode_ = RunMode::HELP;
    } else if (arg1 == "--version" || arg1 == "-v") {
        mode_ = RunMode::VERSION;
    } else if (argc == 2 && arg1[0] != '-') { // Assume it's a filename if not starting with '-'
        // TODO: Add proper file existence check?
        mode_ = RunMode::FILE;
        filename_ = arg1;
    } else {
        // Unrecognized arguments or unsupported combinations
        mode_ = RunMode::ERROR;
        // Store a generic error message, could be more specific based on argv
        error_message_ = "Invalid arguments or unsupported option combination.";
    }
    // Note: This parser is basic. It doesn't handle combined flags,
    // options after filename (e.g., `tooi somefile -v`), or multiple files.
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
 * Prints the standard help message to standard error.
 */
void ArgsParser::show_help(const char* program_name) const {
    std::cerr << "Usage: " << program_name << " [options] [file]\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  -h, --help     Show this help message and exit\n";
    std::cerr << "  -v, --version  Show version information and exit\n";
    std::cerr << "\nArguments:\n";
    std::cerr << "  file           Execute the script from the specified file\n";
    std::cerr << "\nIf no file is provided or only options are given, tooi starts in REPL mode\n";
    std::cerr << "unless -h or -v is specified.\n";
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
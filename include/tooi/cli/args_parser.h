#pragma once

#include <string>
#include <vector>

/**
 * @namespace tooi
 * @brief The main namespace for the Tooi project.
 */
namespace tooi {
/**
 * @namespace tooi::cli
 * @brief Contains classes and functions related to the command-line interface.
 */
namespace cli {

/**
 * @enum RunMode
 * @brief Defines the operational mode of the interpreter based on command-line arguments.
 */
enum class RunMode {
    HELP,     ///< Show the help message and exit.
    VERSION,  ///< Show the version information and exit.
    REPL,     ///< Run in Read-Eval-Print Loop mode.
    FILE,     ///< Execute commands from a specified file.
    ERROR     ///< An error occurred during argument parsing.
};

/**
 * @class ArgsParser
 * @brief Parses command-line arguments to determine the run mode and options.
 */
class ArgsParser {
public:
    /**
     * @brief Parses the command-line arguments provided to the program.
     * @param argc The argument count.
     * @param argv The argument vector (array of C-style strings).
     */
    void parse(int argc, char *argv[]);

    /**
     * @brief Gets the determined run mode after parsing.
     * @return The determined RunMode.
     */
    RunMode get_mode() const;

    /**
     * @brief Gets the filename provided via command-line arguments, if any.
     * @return A constant reference to the filename string. Returns an empty string if no file was
     * specified or if the mode is not FILE.
     */
    const std::string &get_filename() const;

    /**
     * @brief Checks if verbose mode was requested.
     * @return True if --verbose flag was present, false otherwise.
     */
    bool is_verbose() const;

    /**
     * @brief Displays a standard help message to the console.
     * @param program_name The name of the executable (argv[0]), used in the help message.
     */
    void show_help(const char *program_name) const;

    /**
     * @brief Displays the program's version information to the console.
     */
    void show_version() const;

private:
    RunMode mode_ = RunMode::REPL;  ///< @brief The determined run mode. Defaults to REPL.
    std::string filename_;          ///< @brief Stores the filename if provided.
    std::string error_message_;     ///< @brief Stores any error message encountered during parsing.
    bool verbose_ = false;          ///< @brief Flag for verbose output.
};

}  // namespace cli
}  // namespace tooi
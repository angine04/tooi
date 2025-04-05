/**
 * @file run_from_file.cpp
 * @brief Implementation of the run_from_file function.
 */
#include "tooi/cli/run_from_file.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem> // Include for filesystem operations
#include "tooi/core/interpreter.h"

namespace tooi {
namespace cli {

/**
 * @brief Executes a Tooi script from the specified file.
 *
 * Checks file validity, opens the file, creates an interpreter instance,
 * and runs the interpreter using the file's content as input.
 *
 * @param filename The path to the script file.
 * @param verbose Flag indicating whether to print verbose output.
 * @return True if the file was valid and the script executed without fatal
 *         interpreter errors, false otherwise.
 */
bool run_from_file(const std::string& filename, bool verbose) {
    // 1. Check file validity using <filesystem>
    std::error_code ec;
    auto status = std::filesystem::status(filename, ec);

    if (ec) {
        // Error occurred during status check (e.g., permissions issue)
        std::cerr << "Error: Cannot access file status for \"" << filename << "\": " << ec.message() << std::endl;
        return false;
    }

    if (!std::filesystem::exists(status)) {
        std::cerr << "Error: File does not exist: \"" << filename << "\"" << std::endl;
        return false;
    }

    if (!std::filesystem::is_regular_file(status)) {
        // It exists, but it's not a regular file (could be directory, symlink etc.)
        std::cerr << "Error: Path is not a regular file: \"" << filename << "\"" << std::endl;
        return false;
    }

    // 2. Attempt to open the file (read permissions check happens here too)
    std::ifstream file(filename);
    if (!file.is_open()) {
        // This might indicate a read permission error if exists() and is_regular_file() passed
        std::cerr << "Error: Could not open file (check read permissions?): \"" << filename << "\"" << std::endl;
        return false;
    }

    // 3. Run the interpreter
    core::Interpreter interpreter(verbose);
    bool success = interpreter.run(file);

    // file stream is automatically closed when it goes out of scope (RAII)

    if (!success) {
        // Interpreter error messages might be guarded by verbose flag inside run()
        // No need for extra message here unless desired
        return false;
    }

    return true; // Indicate successful file processing and script execution
}

} // namespace cli
} // namespace tooi 
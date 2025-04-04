/**
 * @file run_from_file.cpp
 * @brief Implementation of the run_from_file function.
 */
#include "tooi/cli/run_from_file.h"

#include <fstream>
#include <iostream>
#include <string>
#include "tooi/core/interpreter.h"

namespace tooi {
namespace cli {

/**
 * @brief Executes a Tooi script from the specified file.
 *
 * Opens the file, creates an interpreter instance, and runs the interpreter
 * using the file's content as input.
 *
 * @param filename The path to the script file.
 * @return True if the file was opened and the script executed without fatal
 *         interpreter errors, false otherwise (e.g., file not found, fatal
 *         interpreter error).
 */
bool run_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"" << std::endl;
        return false; // Indicate failure to open
    }

    core::Interpreter interpreter; // Create the interpreter
    bool success = interpreter.run(file); // Run using the file stream

    // file stream is automatically closed when it goes out of scope (RAII)

    if (!success) {
        // Interpreter::run already prints stream errors if they occur during read.
        // We might add a generic message here if needed, but maybe redundant.
        // std::cerr << "Error: Failed executing script in file: " << filename << std::endl;
        return false; // Indicate interpreter failure (could be stream read or fatal parse/exec error)
    }

    return true; // Indicate successful file processing and script execution
}

} // namespace cli
} // namespace tooi 
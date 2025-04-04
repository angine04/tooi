#pragma once

#include <string> // Needed for filename

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
bool run_from_file(const std::string& filename);

} // namespace cli
} // namespace tooi 
#pragma once

// #ifndef TOOI_CORE_INTERPRETER_H
// #define TOOI_CORE_INTERPRETER_H

#include <string>

/**
 * @namespace tooi
 * @brief The main namespace for the Tooi project.
 */
namespace tooi {
/**
 * @namespace tooi::core
 * @brief Contains the core interpreter logic.
 */
namespace core {

/**
 * @class Interpreter
 * @brief The main class responsible for executing Tooi scripts.
 */
class Interpreter {
public:
    /**
     * @brief Executes the script contained within the specified file.
     * @param filename The path to the script file to execute.
     * @return True if execution was successful, false otherwise.
     */
    bool run_file(const std::string& filename);

    // We might add run_string, etc. later
};

}  // namespace core
}  // namespace tooi

// #endif // TOOI_CORE_INTERPRETER_H
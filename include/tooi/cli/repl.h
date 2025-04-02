#pragma once

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
 * @class Repl
 * @brief Manages the Read-Eval-Print Loop (REPL) execution mode.
 */
class Repl {
public:
    /**
     * @brief Starts the REPL, processing user input until termination.
     */
    void run();
};

} // namespace cli
} // namespace tooi 
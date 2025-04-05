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
     * @brief Constructs a Repl instance.
     * @param verbose Enable verbose output during execution.
     */
    explicit Repl(bool verbose = false);

    /**
     * @brief Starts the REPL, processing user input until termination.
     */
    void run();

private:
    bool verbose_ = false; // Flag for verbose output
};

} // namespace cli
} // namespace tooi 
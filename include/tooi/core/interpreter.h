#pragma once

#include <istream> // Include for std::istream
#include <string>
// #include <vector> // Example placeholder for state
// #include <unordered_map> // Example placeholder for state

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
 *
 * This class maintains the execution context (variables, functions, etc.)
 * across multiple calls to run(), allowing for stateful interpretation,
 * suitable for REPL sessions or executing scripts piece by piece.
 */
class Interpreter {
   public:
    /**
     * @brief Executes Tooi code read from the given input stream.
     *
     * Reads code from the stream, parses it, and executes it within the
     * interpreter's current context. Modifies the interpreter's state
     * (e.g., defines variables).
     *
     * @param input_stream The input stream providing the Tooi code.
     * @return True if the code from the stream was processed without fatal
     *         errors (e.g., syntax errors preventing further execution).
     *         Returns false if a fatal error occurred. Non-fatal runtime
     *         errors might still occur and be reported without returning false.
     */
    bool run(std::istream& input_stream);

   private:
    // Placeholder for interpreter state:
    // std::unordered_map<std::string, Value> variables_;
    // ExecutionEnvironment environment_;
    int execution_count_ = 0; // Simple example of state
};

}  // namespace core
}  // namespace tooi

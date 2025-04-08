#pragma once

#include "tooi/core/error_info.h" // Includes ErrorCode and ErrorInfo
#include <unordered_map>
#include <mutex> // For thread-safe singleton initialization

namespace tooi {
namespace core {

/**
 * @brief A singleton registry to manage and retrieve error information.
 *
 * This class holds the definitions (code string, message format, description)
 * for all ErrorCodes used in the system. Currently initialized internally,
 * but designed to potentially load from an external source (e.g., JSON) later.
 */
class ErrorRegistry {
public:
    // Deleted copy constructor and assignment operator for singleton
    ErrorRegistry(const ErrorRegistry&) = delete;
    ErrorRegistry& operator=(const ErrorRegistry&) = delete;

    /**
     * @brief Gets the singleton instance of the ErrorRegistry.
     * Thread-safe initialization.
     * @return Reference to the singleton instance.
     */
    static ErrorRegistry& instance();

    /**
     * @brief Retrieves the ErrorInfo structure for a given ErrorCode.
     * @param code The ErrorCode to look up.
     * @return A constant reference to the corresponding ErrorInfo.
     * @throws ErrorRegistryError if the code is not found in the registry.
     */
    const ErrorInfo& get_info(ErrorCode code) const;

    // Public destructor needed for unique_ptr with private constructor
    ~ErrorRegistry() = default;

private:
    // Private constructor for singleton pattern
    ErrorRegistry();

    /**
     * @brief Initializes the registry with hardcoded error definitions.
     * Called by the constructor. Will be replaced by loading logic later.
     */
    void initialize_registry();

    std::unordered_map<ErrorCode, ErrorInfo> registry_map_;
    static std::unique_ptr<ErrorRegistry> instance_;
    static std::once_flag init_flag_;
};

} // namespace core
} // namespace tooi

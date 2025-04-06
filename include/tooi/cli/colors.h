#pragma once

namespace tooi {
namespace cli {

/**
 * @brief Provides ANSI escape codes for terminal colors.
 * 
 * Usage: std::cout << colors::BOLD_RED << "Error!" << colors::RESET << std::endl;
 * Ensure terminal supports ANSI codes. For Windows cmd.exe, enable virtual terminal processing.
 */
namespace colors {

// Reset attribute
inline const char* const RESET = "\x1B[0m";

// Regular Colors
inline const char* const BLACK = "\x1B[0;30m";
inline const char* const RED = "\x1B[0;31m";
inline const char* const GREEN = "\x1B[0;32m";
inline const char* const YELLOW = "\x1B[0;33m";
inline const char* const BLUE = "\x1B[0;34m";
inline const char* const MAGENTA = "\x1B[0;35m";
inline const char* const CYAN = "\x1B[0;36m";
inline const char* const WHITE = "\x1B[0;37m";

// Bold Colors
inline const char* const BOLD_BLACK = "\x1B[1;30m";
inline const char* const BOLD_RED = "\x1B[1;31m";
inline const char* const BOLD_GREEN = "\x1B[1;32m";
inline const char* const BOLD_YELLOW = "\x1B[1;33m";
inline const char* const BOLD_BLUE = "\x1B[1;34m";
inline const char* const BOLD_MAGENTA = "\x1B[1;35m";
inline const char* const BOLD_CYAN = "\x1B[1;36m";
inline const char* const BOLD_WHITE = "\x1B[1;37m";

// Add other styles like underline, background if needed

} // namespace colors
} // namespace cli
} // namespace tooi 
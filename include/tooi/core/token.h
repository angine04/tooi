#pragma once

#include <cstdint> // For integer types
#include <string>
#include <variant>
#include <vector>

namespace tooi {
namespace core {

/**
 * @brief Represents the different types of tokens in the Tooi language
 */
enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, ASTERISK,
    AT, QUOTE, HASHTAG, DOLLAR, QUESTION, COLON,
    CARET, PERCENT, AMPERSAND, PIPE, TILDE,
    // Quote is not currently supported as a token type. Reserved.

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    COLON_COLON,
    MINUS_GREATER, EQUAL_GREATER,
    GREATER_GREATER,

    // Literals.
    IDENTIFIER_LITERAL, STRING_LITERAL, NUMBER_LITERAL,

    // Keywords.
    IF, ELSE, FOR, WHILE, DONE, SKIP,
    TRUE, FALSE, NIL, AND, OR, NOT,
    ADD, EXPORT, WITH, SELF, AS, CALL,
    LET, SET, NEW, DO, BE, OF, IN,
    PUBLIC, PRIVATE, RUNNABLE, PURE, PARAM,

    INT, FLOAT, BYTE, STRING, BOOL, UINT, PROTO,
    INT32, INT64, UINT32, UINT64, FLOAT32, FLOAT64,

    ERROR,      ///< Represents a lexical error
    END_OF_FILE ///< Represents end of input
};

/**
 * @brief Type alias for token literal values
 * 
 * Can hold various numeric types, strings, or be empty via std::monostate
 */
using TokenLiteral = std::variant<
    std::monostate,  // Represents no literal value
    std::string,   // For string literals and identifiers
    uint64_t,      // For all integer literals (magnitude)
    double         // For all floating-point literals
>;

/**
 * @brief Represents a single token in the source code
 */
struct Token {
    TokenType type;      ///< The type of this token
    std::string lexeme;  ///< The actual character sequence from the source
    TokenLiteral literal;///< The literal value, if any
    int line;           ///< Line number for error reporting

    /**
     * @brief Constructs a new Token
     * @param type The token type
     * @param lexeme The lexeme string
     * @param literal The literal value
     * @param line The line number
     */
    Token(TokenType type, std::string lexeme, TokenLiteral literal, int line)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    /**
     * @brief Creates an EOF token
     * @param line The line number
     * @return Token An EOF token
     */
    static Token make_eof(int line) {
        return Token(TokenType::END_OF_FILE, "", std::monostate{}, line);
    }

    /**
     * @brief Converts the token to a string representation
     * @return std::string The string representation
     */
    std::string to_string() const;

    /**
     * @brief Converts a TokenType enum value to its string representation.
     * @param type The TokenType value.
     * @return const char* The string name of the token type.
     */
    static const char* type_to_string(TokenType type);
};

} // namespace core
} // namespace tooi 
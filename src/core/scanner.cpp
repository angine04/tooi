#include "tooi/core/scanner.h"
#include "tooi/core/token.h"  // Include Token definitions (needs to be before use)

#include <iostream>   // For error reporting
#include <sstream>    // For Token::to_string
#include <stdexcept>  // For std::stod errors
#include <unordered_map>
#include <utility>  // For std::move

namespace tooi {
namespace core {

namespace {
// Keyword map for easy lookup
const std::unordered_map<std::string, TokenType> keywords = {
    // Control flow & Boolean
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"done", TokenType::DONE},
    {"skip", TokenType::SKIP},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"nil", TokenType::NIL},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},

    // Operations & Declarations
    {"add", TokenType::ADD},
    {"export", TokenType::EXPORT},
    {"with", TokenType::WITH},
    {"self", TokenType::SELF},
    {"as", TokenType::AS},
    {"call", TokenType::CALL},
    {"let", TokenType::LET},
    {"set", TokenType::SET},
    {"new", TokenType::NEW},
    {"do", TokenType::DO},
    {"be", TokenType::BE},
    {"of", TokenType::OF},
    {"in", TokenType::IN},

    // Modifiers & Parameters
    {"public", TokenType::PUBLIC},
    {"private", TokenType::PRIVATE},
    {"runnable", TokenType::RUNNABLE},
    {"pure", TokenType::PURE},
    {"param", TokenType::PARAM},

    // Basic Types
    {"int", TokenType::INT},
    {"float", TokenType::FLOAT},
    {"byte", TokenType::BYTE},
    {"string", TokenType::STRING},
    {"bool", TokenType::BOOL},
    {"uint", TokenType::UINT},
    {"proto", TokenType::PROTO},

    // Specific Width Types
    {"int32", TokenType::INT32},
    {"int64", TokenType::INT64},
    {"uint32", TokenType::UINT32},
    {"uint64", TokenType::UINT64},
    {"float32", TokenType::FLOAT32},
    {"float64", TokenType::FLOAT64}};

// Helper functions for character checks
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alpha_numeric(char c) {
    return is_alpha(c) || is_digit(c);
}

}  // anonymous namespace

// --- Token Implementation ---

// Implementation of the static helper method
const char* Token::type_to_string(TokenType type) {
    switch (type) {
        // Single-character tokens.
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET:
            return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET:
            return "RIGHT_BRACKET";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::ASTERISK:
            return "ASTERISK";
        case TokenType::AT:
            return "AT";
        case TokenType::QUOTE:
            return "QUOTE";
        case TokenType::HASHTAG:
            return "HASHTAG";
        case TokenType::DOLLAR:
            return "DOLLAR";
        case TokenType::QUESTION:
            return "QUESTION";
        case TokenType::COLON:
            return "COLON";
        // One or two character tokens.
        case TokenType::BANG:
            return "BANG";
        case TokenType::BANG_EQUAL:
            return "BANG_EQUAL";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case TokenType::LESS:
            return "LESS";
        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";
        case TokenType::COLON_COLON:
            return "COLON_COLON";
        case TokenType::MINUS_GREATER:
            return "MINUS_GREATER";
        case TokenType::EQUAL_GREATER:
            return "EQUAL_GREATER";
        case TokenType::GREATER_GREATER:
            return "GREATER_GREATER";
        // Literals.
        case TokenType::IDENTIFIER_LITERAL:
            return "IDENTIFIER_LITERAL";
        case TokenType::STRING_LITERAL:
            return "STRING_LITERAL";
        case TokenType::NUMBER_LITERAL:
            return "NUMBER_LITERAL";
        // Keywords.
        case TokenType::IF:
            return "IF";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::FOR:
            return "FOR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::DONE:
            return "DONE";
        case TokenType::SKIP:
            return "SKIP";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::NIL:
            return "NIL";
        case TokenType::AND:
            return "AND";
        case TokenType::OR:
            return "OR";
        case TokenType::NOT:
            return "NOT";
        case TokenType::ADD:
            return "ADD";
        case TokenType::EXPORT:
            return "EXPORT";
        case TokenType::WITH:
            return "WITH";
        case TokenType::SELF:
            return "SELF";
        case TokenType::AS:
            return "AS";
        case TokenType::CALL:
            return "CALL";
        case TokenType::LET:
            return "LET";
        case TokenType::SET:
            return "SET";
        case TokenType::NEW:
            return "NEW";
        case TokenType::DO:
            return "DO";
        case TokenType::BE:
            return "BE";
        case TokenType::OF:
            return "OF";
        case TokenType::IN:
            return "IN";
        case TokenType::PUBLIC:
            return "PUBLIC";
        case TokenType::PRIVATE:
            return "PRIVATE";
        case TokenType::RUNNABLE:
            return "RUNNABLE";
        case TokenType::PURE:
            return "PURE";
        case TokenType::PARAM:
            return "PARAM";
        case TokenType::INT:
            return "INT";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::BYTE:
            return "BYTE";
        case TokenType::STRING:
            return "STRING";
        case TokenType::BOOL:
            return "BOOL";
        case TokenType::UINT:
            return "UINT";
        case TokenType::PROTO:
            return "PROTO";
        case TokenType::INT32:
            return "INT32";
        case TokenType::INT64:
            return "INT64";
        case TokenType::UINT32:
            return "UINT32";
        case TokenType::UINT64:
            return "UINT64";
        case TokenType::FLOAT32:
            return "FLOAT32";
        case TokenType::FLOAT64:
            return "FLOAT64";
        // Other.
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::END_OF_FILE:
            return "END_OF_FILE";
        default:
            return "<Unknown Type>";
    }
}

std::string Token::to_string() const {
    std::stringstream ss;
    ss << "Token [Type: " << Token::type_to_string(type) << ", Lexeme: '" << lexeme
       << "', Literal: ";

    std::visit(
        [&ss](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                ss << "<none>";
            } else if constexpr (std::is_same_v<T, std::string>) {
                ss << '\"' << arg << '\"';  // Print strings in quotes
            } else if constexpr (std::is_same_v<T, int32_t>) {
                ss << arg << "i32";  // Add suffix for clarity
            } else if constexpr (std::is_same_v<T, int64_t>) {
                ss << arg << "i64";  // Add suffix for clarity
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                ss << arg << "u32";  // Add suffix for clarity
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                ss << arg << "u64";  // Add suffix for clarity
            } else if constexpr (std::is_same_v<T, float>) {
                ss << arg << "f";  // Add suffix for clarity
            } else if constexpr (std::is_same_v<T, double>) {
                ss << arg << "d";  // Add suffix for clarity (or just arg if default)
            } else {
                ss << "<unknown literal type>";
            }
        },
        literal);

    ss << ", Line: " << line << "]";
    return ss.str();
}

// --- Scanner Implementation ---

Scanner::Scanner(std::string source) : source_(std::move(source)) {}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        // We are at the beginning of the next lexeme.
        start_ = current_;
        scan_token();
    }

    tokens_.push_back(Token::make_eof(line_));
    return tokens_;  // Consider returning a const ref or moving tokens_
}

bool Scanner::is_at_end() const {
    return current_ >= source_.length();
}

char Scanner::advance() {
    return source_[current_++];
}

void Scanner::add_token(TokenType type) {
    add_token(type, std::monostate{});
}

void Scanner::add_token(TokenType type, const TokenLiteral& literal) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.emplace_back(type, std::move(text), literal, line_);
}

bool Scanner::match(char expected) {
    if (is_at_end())
        return false;
    if (source_[current_] != expected)
        return false;
    current_++;
    return true;
}

char Scanner::peek() const {
    if (is_at_end())
        return '\0';
    return source_[current_];
}

char Scanner::peek_next() const {
    if (current_ + 1 >= source_.length())
        return '\0';
    return source_[current_ + 1];
}

void Scanner::skip_whitespace_and_comments() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                advance();
                break;
            case '\n':
                line_++;
                advance();
                break;
            case '/':
                if (peek_next() == '/') {
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !is_at_end())
                        advance();
                } else if (peek_next() == '*') {
                    // C-style block comment (Bonus)
                    advance();  // Consume /*
                    advance();
                    int nesting = 1;
                    while (nesting > 0 && !is_at_end()) {
                        if (peek() == '\n')
                            line_++;
                        if (peek() == '/' && peek_next() == '*') {
                            advance();
                            advance();
                            nesting++;
                        } else if (peek() == '*' && peek_next() == '/') {
                            advance();
                            advance();
                            nesting--;
                        } else {
                            advance();
                        }
                    }
                    if (nesting > 0) {
                        // Unterminated block comment - report error?
                        // For now, just continue scanning after the unterminated part
                    }
                } else {
                    return;  // It's just a slash, not a comment
                }
                break;
            default:
                return;
        }
    }
}

void Scanner::scan_string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n')
            line_++;  // Allow multi-line strings
        // Handle escape sequences if needed (e.g., \n, \t, \", \\)
        // if (peek() == '\\' && peek_next() == 'n') { ... }
        advance();
    }

    if (is_at_end()) {
        // Unterminated string.
        // Report error - e.g., create an ERROR token
        std::cerr << "Line " << line_ << ": Error: Unterminated string." << std::endl;
        add_token(TokenType::ERROR);  // Or specific error token
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes and create literal.
    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    // TODO: Process escape sequences in 'value' here if implemented
    add_token(TokenType::STRING, value);
}

void Scanner::scan_number() {
    while (is_digit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && is_digit(peek_next())) {
        // Consume the "."
        advance();
        while (is_digit(peek()))
            advance();
    }

    // Convert the substring to a double
    // Consider adding error handling for std::stod
    std::string num_str = source_.substr(start_, current_ - start_);
    try {
        double value = std::stod(num_str);
        add_token(TokenType::NUMBER_LITERAL, value);
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Line " << line_ << ": Error: Invalid number format: " << num_str << std::endl;
        add_token(TokenType::ERROR);  // Or specific error token
    } catch (const std::out_of_range& oor) {
        std::cerr << "Line " << line_ << ": Error: Number out of range: " << num_str << std::endl;
        add_token(TokenType::ERROR);  // Or specific error token
    }
}

void Scanner::scan_identifier() {
    while (is_alpha_numeric(peek()))
        advance();

    std::string text = source_.substr(start_, current_ - start_);
    TokenType type;
    auto it = keywords.find(text);
    if (it == keywords.end()) {
        type = TokenType::IDENTIFIER_LITERAL;
    } else {
        type = it->second;
    }
    add_token(type);
}

void Scanner::scan_token() {
    skip_whitespace_and_comments();
    start_ = current_;  // Reset start after skipping whitespace/comments

    if (is_at_end())
        return;  // Nothing more to scan

    char c = advance();

    if (is_alpha(c)) {
        scan_identifier();
        return;
    }
    if (is_digit(c)) {
        scan_number();
        return;
    }

    switch (c) {
        case '(':
            add_token(TokenType::LEFT_PAREN);
            break;
        case ')':
            add_token(TokenType::RIGHT_PAREN);
            break;
        case '{':
            add_token(TokenType::LEFT_BRACE);
            break;
        case '}':
            add_token(TokenType::RIGHT_BRACE);
            break;
        case ',':
            add_token(TokenType::COMMA);
            break;
        case '.':
            add_token(TokenType::DOT);
            break;
        case '-':
            add_token(match('>') ? TokenType::MINUS_GREATER : TokenType::MINUS);
            break;
        case '+':
            add_token(TokenType::PLUS);
            break;
        case ';':
            add_token(TokenType::SEMICOLON);
            break;
        case '*':
            add_token(TokenType::ASTERISK);
            break;
        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            if (match('>')) {
                add_token(TokenType::EQUAL_GREATER);
            } else {
                add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            }
            break;
        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            if (match('=')) {
                add_token(TokenType::GREATER_EQUAL);
            } else if (match('>')) {
                add_token(TokenType::GREATER_GREATER);
            } else {
                add_token(TokenType::GREATER);
            }
            break;
        case '/':  // Slash was not part of a comment
            add_token(TokenType::SLASH);
            break;

        case '"':
            scan_string();
            break;

        case ':':
            add_token(match(':') ? TokenType::COLON_COLON : TokenType::COLON);
            break;

        default:
            // Report error for unexpected character.
            std::cerr << "Line " << line_ << ": Error: Unexpected character '" << c << "'"
                      << std::endl;
            add_token(TokenType::ERROR);  // Or specific error token
            break;
    }
}

}  // namespace core
}  // namespace tooi
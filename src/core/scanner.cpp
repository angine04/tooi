#include "tooi/core/scanner.h"
#include "tooi/core/token.h"  // Include Token definitions (needs to be before use)
#include "tooi/cli/colors.h" // Include colors

#include <iostream>   // For error reporting
#include <sstream>    // For Token::to_string
#include <stdexcept>  // For std::stod errors
#include <unordered_map>
#include <utility>  // For std::move
#include <cctype>   // For std::isspace

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
        case TokenType::CARET:
            return "CARET";
        case TokenType::PERCENT:
            return "PERCENT";
        case TokenType::AMPERSAND:
            return "AMPERSAND";
        case TokenType::PIPE:
            return "PIPE";
        case TokenType::TILDE:
            return "TILDE";
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
    using namespace tooi::cli::colors; // Using declaration
    std::stringstream ss;
    // Apply colors: Cyan for Type, Yellow for Lexeme, Green for Literal
    ss << "Token [Type: " << BOLD_CYAN << Token::type_to_string(type) << RESET
       << ", Lexeme: '" << YELLOW << lexeme << RESET << "', Literal: " << BOLD_GREEN;

    std::visit([&ss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            ss << "<none>";
        } else if constexpr (std::is_same_v<T, std::string>) {
            ss << '\"' << arg << '\"';
        } else if constexpr (std::is_same_v<T, int32_t>) {
            ss << arg << "i32";
        } else if constexpr (std::is_same_v<T, int64_t>) {
            ss << arg << "i64";
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            ss << arg << "u32";
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            ss << arg << "u64";
        } else if constexpr (std::is_same_v<T, float>) {
            ss << arg << "f";
        } else if constexpr (std::is_same_v<T, double>) {
            ss << arg << "d";
        } else {
             ss << "<unknown literal type>";
        }
    }, literal);

    ss << RESET << ", Line: " << line << "]"; // Reset after literal
    return ss.str();
}

// --- Scanner Implementation ---

Scanner::Scanner(std::string source, ErrorReporter& error_reporter)
    : source_(std::move(source)), error_reporter_(error_reporter) {}

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

// Helper to report error at the current scanning position (start_ to current_)
void Scanner::report_error_here(int length, const std::string& message) {
    // Find the end of the current line
    size_t line_end = source_.find('\n', line_start_);
    if (line_end == std::string::npos) {
        line_end = source_.length(); // Handle last line
    }
    std::string source_line = source_.substr(line_start_, line_end - line_start_);

    // Calculate column (1-based)
    // Use 'start_' as the beginning of the problematic token/char
    int column = (start_ - line_start_) + 1;

    error_reporter_.report_at(line_, column, length, source_line, message);
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
                line_start_ = current_; // Update line start index
                break;
            case '/':
                if (peek_next() == '/') {
                    // Single-line comment
                    while (peek() != '\n' && !is_at_end()) advance();
                } else if (peek_next() == '*') {
                    // Simplified C-style block comment (no nesting)
                    advance(); // Consume /*
                    advance();
                    int comment_start_line = line_; // For error reporting
                    int comment_start_char = current_ - 2; // Position of opening /*

                    while (!is_at_end()) {
                        if (peek() == '*' && peek_next() == '/') {
                            advance(); // Consume */
                            advance();
                            break; // Found the end
                        }
                        if (peek() == '\n') {
                            line_++;
                            line_start_ = current_ + 1; // Update line start after newline
                        }
                        advance(); // Consume character inside comment
                    }

                    if (is_at_end()) {
                        // Unterminated comment - report error at the start of the comment
                        // Need to recalculate line content for the original line
                        size_t err_line_end = source_.find('\n', comment_start_char);
                        if (err_line_end == std::string::npos) err_line_end = source_.length();
                        size_t err_line_start = source_.rfind('\n', comment_start_char);
                        if (err_line_start == std::string::npos) err_line_start = 0; else err_line_start++;
                        std::string err_line = source_.substr(err_line_start, err_line_end - err_line_start);
                        int err_column = (comment_start_char - err_line_start) + 1;
                        error_reporter_.report_at(comment_start_line, err_column, 2, err_line, "Unterminated block comment.");
                         // No need to add ERROR token here, just report and continue scanning after
                    }
                    // IMPORTANT: After handling comment, continue the outer loop
                    // to potentially skip more whitespace/comments that followed immediately
                    // This continue replaces the implicit fallthrough/break behaviour
                    continue; 
                } else {
                    return; // It's just a slash, not a comment
                }
                break; // Break from the switch after handling // or /*
            default:
                return;
        }
    }
}

// Handles strings with escape sequences ("..." or '...')
void Scanner::scan_interpolated_string(char delimiter) {
    std::stringstream value_builder;
    while (peek() != delimiter && !is_at_end()) {
        char c = peek();
        if (c == '\\') { // Escape sequence
            advance(); // Consume '\'
            if (is_at_end()) { // Escaped EOF
                report_error_here(1, "Unterminated escape sequence.");
                add_token(TokenType::ERROR);
                return;
            }
            char escaped = advance(); // Consume character after '\'
            switch (escaped) {
                case 'n':  value_builder << '\n'; break;
                case 't':  value_builder << '\t'; break;
                case '\\': value_builder << '\\'; break;
                case '"': value_builder << '"'; break;
                case '\'': value_builder << '\''; break;
                // Add other escapes like \r, \b, \f if needed
                default:
                    // Report error at the invalid escaped char (current_ - 1)
                    report_error_here(1, "Invalid escape sequence.");
                    value_builder << '\\';
                    value_builder << escaped;
                    break;
            }
        } else {
            if (c == '\n') line_++; // Still track line numbers
            value_builder << c;
            advance();
        }
    }

    if (is_at_end()) {
        report_error_here(1, "Unterminated string literal.");
        add_token(TokenType::ERROR);
        return;
    }

    // The closing delimiter.
    advance();

    add_token(TokenType::STRING_LITERAL, value_builder.str());
}

// Handles raw strings (`...`) without escape sequences
void Scanner::scan_raw_string() {
    while (peek() != '`' && !is_at_end()) {
        if (peek() == '\n') line_++;
        advance();
    }

    if (is_at_end()) {
        report_error_here(1, "Unterminated raw string literal.");
        add_token(TokenType::ERROR);
        return;
    }

    // The closing backtick.
    advance();

    // Trim the surrounding backticks.
    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    add_token(TokenType::STRING_LITERAL, value);
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
        report_error_here(num_str.length(), "Invalid number format.");
        add_token(TokenType::ERROR);
    } catch (const std::out_of_range& oor) {
        report_error_here(num_str.length(), "Number out of range.");
        add_token(TokenType::ERROR);
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
    start_ = current_;

    if (is_at_end()) return;

    char c = advance();

    if (is_alpha(c)) { scan_identifier(); return; }
    if (is_digit(c)) { scan_number(); return; }

    switch (c) {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case '[': add_token(TokenType::LEFT_BRACKET); break;
        case ']': add_token(TokenType::RIGHT_BRACKET); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(match('>') ? TokenType::MINUS_GREATER : TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::ASTERISK); break;
        case '@': add_token(TokenType::AT); break;
        case '#': add_token(TokenType::HASHTAG); break;
        case '$': add_token(TokenType::DOLLAR); break;
        case '?': add_token(TokenType::QUESTION); break;
        case ':': add_token(match(':') ? TokenType::COLON_COLON : TokenType::COLON); break;
        case '^': add_token(TokenType::CARET); break;
        case '%': add_token(TokenType::PERCENT); break;
        case '&': add_token(TokenType::AMPERSAND); break;
        case '|': add_token(TokenType::PIPE); break;
        case '~': add_token(TokenType::TILDE); break;
        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=':
            if (match('>')) {
                add_token(TokenType::EQUAL_GREATER);
            } else {
                add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            }
            break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>':
            if (match('=')) {
                add_token(TokenType::GREATER_EQUAL);
            } else if (match('>')) {
                add_token(TokenType::GREATER_GREATER);
            } else {
                add_token(TokenType::GREATER);
            }
            break;
        case '/': add_token(TokenType::SLASH); break;

        case '"': scan_interpolated_string('"'); break;
        case '\'': scan_interpolated_string('\''); break;
        case '`': scan_raw_string(); break;

        default: // Handle unexpected character sequence
            // Consume contiguous invalid characters
            while (!is_at_end()) {
                char next_char = peek();
                // Check if the next character *could* start a valid token or is whitespace
                // This check might need refinement depending on exact token rules
                if (std::isspace(static_cast<unsigned char>(next_char)) ||
                    is_alpha(next_char) ||
                    is_digit(next_char) ||
                    std::string("(){}[],.-+;*/@'#$?%^&|~<>!=:`").find(next_char) != std::string::npos)
                {
                    // Next character is whitespace or could start a valid token, stop consuming
                    break;
                }
                 // Otherwise, consume the invalid character as part of the sequence
                advance();
            }

            // Now report the error for the entire consumed sequence
            int length = std::max(1, current_ - start_); // Ensure length is at least 1
            report_error_here(length, "Unexpected character sequence.");
            add_token(TokenType::ERROR);
            break;
    }
}

}  // namespace core
}  // namespace tooi


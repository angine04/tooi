#include "tooi/core/scanner.h"
#include "tooi/cli/colors.h"  // Include colors
#include "tooi/core/token.h"  // Include Token definitions (needs to be before use)

#include <cctype>     // For std::isspace
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
    using namespace tooi::cli::colors;  // Using declaration
    std::stringstream ss;
    // Apply colors: Cyan for Type, Yellow for Lexeme, Green for Literal
    ss << "Token [Type: " << BOLD_CYAN << Token::type_to_string(type) << RESET << ", Lexeme: '"
       << YELLOW << lexeme << RESET << "', Literal: " << BOLD_GREEN;

    std::visit(
        [&ss](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                ss << "<none>";
            } else if constexpr (std::is_same_v<T, std::string>) {
                ss << '"' << arg << '"';
            } else if constexpr (std::is_same_v<T, int32_t>) {
                ss << arg << "i32";
            } else if constexpr (std::is_same_v<T, int64_t>) {
                ss << arg;
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
        },
        literal);

    ss << RESET << ", Line: " << line << "]";  // Reset after literal
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
                line_start_ = current_;  // Update line start index
                break;
            case '/':
                if (peek_next() == '/') {
                    // Single-line comment
                    while (peek() != '\n' && !is_at_end())
                        advance();
                } else if (peek_next() == '*') {
                    // Simplified C-style block comment (no nesting)
                    advance();  // Consume /*
                    advance();
                    int comment_start_line = line_;         // For error reporting
                    int comment_start_char = current_ - 2;  // Position of opening /*

                    while (!is_at_end()) {
                        if (peek() == '*' && peek_next() == '/') {
                            advance();  // Consume */
                            advance();
                            break;  // Found the end
                        }
                        if (peek() == '\n') {
                            line_++;
                            line_start_ = current_ + 1;  // Update line start after newline
                        }
                        advance();  // Consume character inside comment
                    }

                    if (is_at_end()) {
                        // Unterminated comment - report error at the start of the comment
                        // Need to recalculate line content for the original line
                        size_t err_line_end = source_.find('\n', comment_start_char);
                        if (err_line_end == std::string::npos)
                            err_line_end = source_.length();
                        size_t err_line_start = source_.rfind('\n', comment_start_char);
                        if (err_line_start == std::string::npos)
                            err_line_start = 0;
                        else
                            err_line_start++;
                        std::string err_line =
                            source_.substr(err_line_start, err_line_end - err_line_start);
                        int err_column = (comment_start_char - err_line_start) + 1;
                        error_reporter_.report_at(comment_start_line, err_column, 2, err_line,
                                                  ErrorCode::Scanner_UnterminatedBlockComment);
                        // No need to add ERROR token here, just report and continue scanning after
                    }
                    // IMPORTANT: After handling comment, continue the outer loop
                    // to potentially skip more whitespace/comments that followed immediately
                    // This continue replaces the implicit fallthrough/break behaviour
                    continue;
                } else {
                    return;  // It's just a slash, not a comment
                }
                break;  // Break from the switch after handling // or /*
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
        if (c == '\\') {        // Escape sequence
            advance();          // Consume '\'
            if (is_at_end()) {  // Escaped EOF
                report_error_code_here(1, ErrorCode::Scanner_UnterminatedEscapeSequence);
                add_token(TokenType::ERROR);
                return;
            }
            char escaped = advance();  // Consume character after '\'
            switch (escaped) {
                case 'n':
                    value_builder << '\n';
                    break;
                case 't':
                    value_builder << '\t';
                    break;
                case '\\':
                    value_builder << '\\';
                    break;
                case '"':
                    value_builder << '"';
                    break;
                case '\'':
                    value_builder << '\'';
                    break;
                // Add other escapes like \r, \b, \f if needed
                default:
                    // Report error at the invalid escaped char (current_ - 1)
                    report_error_code_here(1, ErrorCode::Scanner_InvalidEscapeSequence);
                    value_builder << '\\';
                    value_builder << escaped;
                    break;
            }
        } else {
            if (c == '\n')
                line_++;  // Still track line numbers
            value_builder << c;
            advance();
        }
    }

    if (is_at_end()) {
        report_error_code_here(1, ErrorCode::Scanner_UnterminatedString);
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
        if (peek() == '\n')
            line_++;
        advance();
    }

    if (is_at_end()) {
        report_error_code_here(1, ErrorCode::Scanner_UnterminatedRawString);
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
    size_t num_start = start_;
    bool has_decimal = false;
    bool has_exponent = false; // TODO: Track exponent if supporting scientific notation
    std::string num_part; // Store digits and the dot

    // Phase 1: Scan digits, single dot, more digits
    while (!is_at_end()) {
        char current_char = peek();
        if (is_digit(current_char)) {
            num_part += advance();
        } else if (current_char == '.') {
            if (has_decimal) { // Error: Second decimal point
                // Consume the second dot and any following digits for the error token
                num_part += advance();
                while(is_digit(peek())) { num_part += advance(); }
                report_error_code_here(num_part.length(), ErrorCode::Scanner_MalformedNumber_MultipleDecimals);
                add_token(TokenType::ERROR);
                return;
            }
            // Check for trailing dot immediately
            if (!is_digit(peek_next())) { 
                // Error: Trailing dot (dot not followed by digit)
                num_part += advance(); // Consume the dot for the error token
                report_error_code_here(num_part.length(), ErrorCode::Scanner_MalformedNumber_TrailingDot);
                add_token(TokenType::ERROR);
                return;
            }
            // First valid decimal point
            has_decimal = true;
            num_part += advance(); // Consume the valid dot
        } else {
            // Not a digit or dot, break to check for suffix/other characters
            break;
        }
    }

    // Phase 2: Scan optional suffix (alphanumeric)
    size_t suffix_start_pos = current_;
    std::string suffix_part;
    while (is_alpha_numeric(peek())) {
        suffix_part += advance();
    }

    // Phase 3: Check for unexpected characters immediately after number/suffix
    // Example: 123# or 1.5foo? (assuming foo is not a valid suffix)
    // This check might be better placed after suffix validation?
    // Let's integrate it with suffix validation for now.

    // Phase 4: Validate Suffix based on presence of decimal
    bool store_as_double = false;
    bool is_integer_form = !has_decimal;
    ErrorCode suffix_error = ErrorCode::NoError; // Placeholder

    if (is_integer_form) {
        // Allowed suffixes: "", i, u, i32, i64, u32, u64
        if (suffix_part.empty() || suffix_part == "i" || suffix_part == "u" || 
            suffix_part == "i32" || suffix_part == "i64" || 
            suffix_part == "u32" || suffix_part == "u64") {
            store_as_double = false; // Store as uint64_t
        } else if (suffix_part == "f" || suffix_part == "d") {
            store_as_double = true; // Integer form, but float suffix -> store as double
        } else {
            suffix_error = ErrorCode::Scanner_InvalidNumericSuffix;
        }
    } else { // has_decimal is true
        // Allowed suffixes: "", f, d
        if (suffix_part.empty() || suffix_part == "f" || suffix_part == "d") {
            store_as_double = true; // Float form -> store as double
        } else {
            // Found an integer suffix (like i32) after a decimal point
            suffix_error = ErrorCode::Scanner_InvalidSuffixForFloat; // Need this ErrorCode
        }
    }

    // Report suffix error if found
    if (suffix_error != ErrorCode::NoError) {
        // The error token should ideally cover the number part + the invalid suffix
        report_error_code_here( (current_ - start_), suffix_error, suffix_part);
        add_token(TokenType::ERROR);
        return;
    }

    // Phase 5: Parse into uint64_t or double
    TokenLiteral literal = std::monostate{};
    bool parse_ok = false;
    try {
        if (store_as_double) {
            double value = std::stod(num_part);
            literal = value;
            parse_ok = true;
        } else {
            // Unsigned Integers: Use stoull, store uint64_t magnitude
            if (suffix_part == "u32" || suffix_part == "u64" || suffix_part == "u") {
                if (has_decimal) { /* Report error Scanner_IntegerSuffixWithDecimal */ return; }
                uint64_t magnitude = std::stoull(num_part); // Parse magnitude
                // Always store the parsed uint64_t magnitude
                literal = magnitude;
                parse_ok = true;
            }
            // Signed Integers ... (logic remains the same)
            else if (suffix_part == "i32" || suffix_part == "i" || suffix_part == "i64" || (suffix_part.empty() && !has_decimal)) {
                uint64_t magnitude = std::stoull(num_part); // Parse magnitude
                literal = magnitude;
                parse_ok = true;
            }
        }

        if (parse_ok) {
            add_token(TokenType::NUMBER_LITERAL, literal);
        }

    } catch (const std::invalid_argument& ia) {
        report_error_code_here(current_ - start_, ErrorCode::Scanner_NumberParseError_Invalid, suffix_part.empty() ? "" : suffix_part);
        add_token(TokenType::ERROR);
    } catch (const std::out_of_range& oor) {
        std::string range_type = suffix_part; // Base type on suffix
        if (suffix_part.empty()) {
            range_type = store_as_double ? "double" : "uint64";
        } else if (suffix_part == "f" || suffix_part == "d") {
            range_type = store_as_double ? "double" : "double_from_int"; // Clarify origin
        } // else suffix is u32, u64 etc.
        report_error_code_here(current_ - start_, ErrorCode::Scanner_NumberParseError_OutOfRange, range_type);
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

    if (is_at_end()) {
        // The main scan_tokens loop adds EOF, so just return
        return; 
    }

    // Peek at the next character to decide the token type
    char c = peek();

    // Check for identifier start first (more common?)
    if (is_alpha(c) || c == '_') { // Allow underscore start?
        scan_identifier();
        return;
    }
    // Check for number start
    if (is_digit(c)) {
        scan_number(); // Let scan_number handle from current_
        return;
    }

    // If not identifier or number, NOW consume the character for single/double char tokens
    advance();

    switch (c) {
        // Single-character tokens
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case '[': add_token(TokenType::LEFT_BRACKET); break;
        case ']': add_token(TokenType::RIGHT_BRACKET); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break; // Standalone dot
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::ASTERISK); break;
        case '@': add_token(TokenType::AT); break;
        case '#': add_token(TokenType::HASHTAG); break;
        case '?': add_token(TokenType::QUESTION); break;
        case '^': add_token(TokenType::CARET); break;
        case '%': add_token(TokenType::PERCENT); break;
        case '&': add_token(TokenType::AMPERSAND); break;
        case '|': add_token(TokenType::PIPE); break;
        case '~': add_token(TokenType::TILDE); break;
        case '$': add_token(TokenType::DOLLAR); break;

        // One or two character tokens
        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': 
            if (match('>')) { add_token(TokenType::EQUAL_GREATER); }
            else { add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); }
            break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': 
            if (match('=')) { add_token(TokenType::GREATER_EQUAL); }
            else if (match('>')) { add_token(TokenType::GREATER_GREATER); }
            else { add_token(TokenType::GREATER); }
            break;
        case '/': add_token(TokenType::SLASH); break; // Comments handled in skip_whitespace
        case '-': add_token(match('>') ? TokenType::MINUS_GREATER : TokenType::MINUS); break;
        case ':': add_token(match(':') ? TokenType::COLON_COLON : TokenType::COLON); break;

        // String literals (already consumed the opening quote)
        case '"': scan_interpolated_string('"'); break;
        case '\'': scan_interpolated_string('\''); break;
        case '`': scan_raw_string(); break;

        default:
            // Unrecognized character
            report_error_code_here(1, ErrorCode::Scanner_InvalidCharacter, c);
            // Consider adding an ERROR token here as well for robustness
            // add_token(TokenType::ERROR);
            break;
    }
}

}  // namespace core
}  // namespace tooi

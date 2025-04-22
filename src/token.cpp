#include "token.hpp"

#include <iostream>
#include <regex>
#include <string>

using namespace std;

const regex ALPHA_REGEX = regex(ALPHA_PATTERN);
const regex ALPHA_NUMERIC_REGEX = regex(ALPHA_NUMERIC_PATTERN);
const regex NUMERIC_REGEX = regex(NUMERIC_PATTERN);
const regex NUMERIC_INCOMPLETE_REGEX = regex(NUMERIC_INCOMPLETE_PATTERN);
const regex LITERAL_REGEX = regex(LITERAL_PATTERN);
const regex LITERAL_INCOMPLETE_REGEX = regex(LITERAL_INCOMPLETE_PATTERN);
const regex COMMENT_LINE_REGEX = regex(COMMENT_LINE_PATTERN);
const regex COMMENT_BLOCK_REGEX = regex(COMMENT_BLOCK_PATTERN);
const regex COMMENT_BLOCK_INCOMPLETE_REGEX = regex(COMMENT_BLOCK_INCOMPLETE_PATTERN);
const regex OPERATORS_REGEX = create_regex(OPERATORS);
const regex SEPARATORS_IGNORED_REGEX = create_regex(SEPARATORS_IGNORED);
const regex SEPARATORS_IMPORTANT_REGEX = create_regex(SEPARATORS_IMPORTANT);

string escape_regex(const string& str) {
    static const regex special_chars(R"([.^$|()\\[*+?{}])");
    return regex_replace(str, special_chars, R"(\$&)");
}

regex create_regex(vector<string> tokens) {
    string regex_string = "";
    for (size_t i = 0; i < tokens.size(); i++) {
        if (i > 0) {
            regex_string += '|';
        }
        regex_string += escape_regex(tokens[i]);
    }

    return regex("^(" + regex_string + ")$");
}

Token get_token(string::iterator& sentinel, string::iterator end, pair<int, int>& position) {
    Token token;
    auto scout = sentinel;

    while (scout != end && regex_search(string(1, *scout), SEPARATORS_IGNORED_REGEX)) {
        position = {position.first + 1, position.second};
        if (*scout == '\n') {
            position = {0, position.second + 1};
        }
        ++scout;
        ++sentinel;
    }
    token.position = position;

    for (; scout != end; ++scout) {
        string actual_char(1, *scout);
        string content_updated = token.content + actual_char;

        if (token.type != LITERAL && token.type != COMMENT && regex_search(actual_char, SEPARATORS_IGNORED_REGEX)) {
            break;
        }

        if (token.type == NONE) {
            if (regex_search(actual_char, ALPHA_REGEX)) {
                token.type = IDENTIFICATOR;
            } else if (regex_search(actual_char, SEPARATORS_IMPORTANT_REGEX)) {
                token.type = SEPARATOR;
                token.content = content_updated;
                break;
            } else if (regex_search(actual_char, OPERATORS_REGEX)) {
                token.type = OPERATOR;
            } else if (regex_search(actual_char, NUMERIC_INCOMPLETE_REGEX)) {
                token.type = NUMBER;
            } else if (regex_search(actual_char, LITERAL_INCOMPLETE_REGEX)) {
                token.type = LITERAL;
            }
        } else if (token.type == IDENTIFICATOR) {
            if (!regex_search(content_updated, ALPHA_NUMERIC_REGEX)) {
                if (regex_search(actual_char, SEPARATORS_IMPORTANT_REGEX) || regex_search(actual_char, OPERATORS_REGEX)) {
                    break;
                }
                token.content = content_updated;
                lexical_error(token);
            }
        } else if (token.type == OPERATOR) {
            if (regex_search(content_updated, COMMENT_LINE_REGEX) || regex_search(content_updated, COMMENT_BLOCK_INCOMPLETE_REGEX)) {
                token.type = COMMENT;
            } else if (!regex_search(content_updated, OPERATORS_REGEX)) {
                if (regex_search(actual_char, SEPARATORS_IMPORTANT_REGEX) || regex_search(actual_char, ALPHA_REGEX) || regex_search(actual_char, NUMERIC_REGEX) || regex_search(actual_char, LITERAL_INCOMPLETE_REGEX)) {
                    break;
                }
                token.content = content_updated;
                lexical_error(token);
            }
        } else if (token.type == NUMBER) {
            if (token.valid && !regex_search(content_updated, NUMERIC_REGEX)) {
                if (regex_search(content_updated, NUMERIC_INCOMPLETE_REGEX)) {
                    token.valid = false;
                } else {
                    token.valid = true;
                    if (regex_search(actual_char, SEPARATORS_IMPORTANT_REGEX) || regex_search(actual_char, OPERATORS_REGEX)) {
                        break;
                    }
                    token.content = content_updated;
                    lexical_error(token);
                }
            }
        } else if (token.type == LITERAL) {
            if (!regex_search(content_updated, LITERAL_INCOMPLETE_REGEX)) {
                if (regex_search(content_updated, LITERAL_REGEX)) {
                    token.content = content_updated;
                    break;
                }
                token.content = content_updated;
                lexical_error(token);
            }
        } else if (token.type == COMMENT) {
            if ((!regex_search(content_updated, COMMENT_LINE_REGEX) && !regex_search(content_updated, COMMENT_BLOCK_INCOMPLETE_REGEX)) || regex_search(content_updated, COMMENT_BLOCK_REGEX)) {
                token.content = content_updated;
                break;
            }
        }
        token.content = content_updated;
    }

    advance(sentinel, token.content.size());
    position = {position.first + token.content.size(), position.second};
    return token;
}

vector<Token> tokenize(string& buffer) {
    vector<Token> tokens;
    auto sentinel = buffer.begin();
    pair<int, int> position = {0, 0};

    while (sentinel != buffer.end()) {
        Token token = get_token(sentinel, buffer.end(), position);
        if (!token.content.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

string enum_type_string(TokenType type) {
    switch (type) {
        case NONE:
            return "NONE";
        case IDENTIFICATOR:
            return "IDENTIFICATOR";
        case NUMBER:
            return "NUMBER";
        case SEPARATOR:
            return "SEPARATOR";
        case OPERATOR:
            return "OPERATOR";
        case LITERAL:
            return "LITERAL";
        case COMMENT:
            return "COMMENT";
        default:
            return "UNKNOWN";
    }
}

void lexical_error(Token token) {
    cout << "Invalid token " << token.content << " at line " << token.position.second << " (" << token.position.first << ")\n";
    exit(1);
}
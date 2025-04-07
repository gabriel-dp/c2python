#include "token.hpp"

#include <iostream>
#include <regex>
#include <string>

using namespace std;

const regex SEPARATORS_IGNORED_REGEX = create_regex(SEPARATORS_IGNORED);
const regex SEPARATORS_IMPORTANT_REGEX = create_regex(SEPARATORS_IMPORTANT);
const regex OPERATORS_REGEX = create_regex(OPERATORS);

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

    return regex("(" + regex_string + ")");
}

string get_token(string::iterator& scout, string::iterator end) {
    string token;

    string actual_char(1, *scout);
    while (scout != end && regex_search(actual_char, SEPARATORS_IGNORED_REGEX)) {
        ++scout;
    }

    for (string actual_char(1, *scout); scout != end;) {
        if (regex_search(actual_char, SEPARATORS_IGNORED_REGEX)) {
            break;
        }
        actual_char = *scout;
        token += *scout;
        ++scout;
    }

    return token;
}

void tokenize(string& buffer) {
    auto scout = buffer.begin();

    while (scout != buffer.end()) {
        string token = get_token(scout, buffer.end());
        if (!token.empty()) {
            cout << token << "\n";
        }
    }
}
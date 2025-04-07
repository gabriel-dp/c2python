#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <regex>
#include <string>
#include <vector>

using namespace std;

const vector<string> SEPARATORS_IGNORED = {" ", "\n", "\t"};
const vector<string> SEPARATORS_IMPORTANT = {"(", ")", "{", "}", "[", "]", ";", ",", "."};
const vector<string> OPERATORS = {"*", "/", "%", "+", "-", "==", "!=", "&&", "||", "=", "+=", "-=", "*=", "/=", "%="};

class Token {
   public:
    string content = "";
    pair<int, int> position = {-1, -1};

    Token(string content, pair<int, int> position) : content(move(content)), position(position) {}
};

void tokenize(string& buffer);
string get_token(string::iterator& scout, string::iterator end);
regex create_regex(vector<string> tokens);

#endif
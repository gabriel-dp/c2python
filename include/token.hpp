#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <regex>
#include <string>
#include <vector>

using namespace std;

enum TokenType {
    NONE,
    IDENTIFICATOR,
    OPERATOR,
    SEPARATOR,
    LITERAL,
    NUMBER,
};

const string ALPHA_PATTERN = "^[A-Za-z_]+$";
const string ALPHA_NUMERIC_PATTERN = "^[A-Za-z0-9_]+$";
const string NUMERIC_PATTERN = "(^[0-9]+(\\.[0-9]+)?f?$)";
const string NUMERIC_INCOMPLETE_PATTERN = "^[0-9]+\\.$";
const string LITERAL_PATTERN = R"(^\"([^\"\\\n]|\\.)*\"$)";
const string LITERAL_INCOMPLETE_PATTERN = R"(^\"([^\"\\\n]|\\.)*$)";
const vector<string> OPERATORS = {"*", "/", "%", "+", "-", "==", "!=", "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "&", "|", ".", ">", "<", ">=", "<=", "!"};
const vector<string> SEPARATORS_IGNORED = {" ", "\n", "\t"};
const vector<string> SEPARATORS_IMPORTANT = {"(", ")", "{", "}", "[", "]", ";", ",", "."};

class Token {
   public:
    string content = "";
    TokenType type = NONE;
    pair<int, int> position = {-1, -1};
    bool valid = true;

    Token() {};
    Token(string content, TokenType type, pair<int, int> position) : content(move(content)), type(type), position(position) {};
};

void lexical_error(Token token);
vector<Token>
tokenize(string& buffer);
Token get_token(string::iterator& sentinel, string::iterator end, pair<int, int>& position);
regex create_regex(vector<string> tokens);
string enum_type_string(TokenType type);

#endif
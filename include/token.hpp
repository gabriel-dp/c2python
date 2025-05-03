#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

enum TokenType {
    NONE,
    KEYWORD,
    IDENTIFICATOR,
    OPERATOR,
    SEPARATOR,
    LITERAL,
    NUMBER,
    COMMENT
};

const string ALPHA_PATTERN = "^[A-Za-z_]+$";
const string ALPHA_NUMERIC_PATTERN = "^[A-Za-z0-9_]+$";
const string NUMERIC_PATTERN = "(^[0-9]+(\\.[0-9]+)?f?$)";
const string NUMERIC_INCOMPLETE_PATTERN = "^[0-9]+\\.$";
const string LITERAL_PATTERN = R"(^\"([^\"\\\n]|\\.)*\"$)";
const string LITERAL_INCOMPLETE_PATTERN = R"(^\"([^\"\\\n]|\\.)*$)";
const string COMMENT_LINE_PATTERN = R"(^//.*)";
const string COMMENT_BLOCK_PATTERN = R"(/\*[\s\S]*?\*/)";
const string COMMENT_BLOCK_INCOMPLETE_PATTERN = R"(/\*[\s\S]*)";
const vector<string> OPERATORS = {"*", "/", "%", "+", "-", "==", "!=", "&&", "||", "=", "+=", "-=", "*=", "/=", "%=", "&", "|", ".", ">", "<", ">=", "<=", "!", "++", "--"};
const vector<string> SEPARATORS_IGNORED = {" ", "\n", "\t"};
const vector<string> SEPARATORS_IMPORTANT = {"(", ")", "{", "}", "[", "]", ";", ",", "."};
const vector<string> KEYWORDS = {"break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "float", "for", "goto", "if", "int", "long", "return", "short", "sizeof", "static", "struct", "switch", "typedef", "void", "while"};

class Token {
   public:
    string content = "";
    TokenType type = NONE;
    pair<int, int> position = {-1, -1};
    bool valid = true;

    Token() {};
    Token(string content, TokenType type, pair<int, int> position) : content(move(content)), type(type), position(position) {};

    void print() {
        cout << "(" << position.first << "," << position.second << ") " << enum_type_string(type) << endl;
        cout << content << "\n\n";
    }

    static string enum_type_string(TokenType type) {
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
            case KEYWORD:
                return "KEYWORD";
            default:
                return "UNKNOWN";
        }
    }
};

void lexical_error(Token token);
vector<Token>
tokenize(string& buffer);
Token get_token(string::iterator& sentinel, string::iterator end, pair<int, int>& position);
regex create_regex(vector<string> tokens);

#endif
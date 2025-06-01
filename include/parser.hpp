#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"

class Parser {
   public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0), level(0) {}
    bool parse();

   private:
    vector<string> TYPES = {"int", "float", "double", "char", "void"};
    vector<string> TYPE_DECORATORS = {"unsigned", "short", "long", "signed"};
    vector<string> OPERATORS_BINARY = {"+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">="};
    vector<string> OPERATORS_UNARY_PREFIX = {"!", "+", "-", "++", "--"};
    vector<string> OPERATORS_UNARY_POSTFIX = {"++", "--"};

    const std::vector<Token>& tokens;
    size_t pos;
    int level = 0;

    Token peek() const;
    Token advance();
    bool match(TokenType type, bool must_advance);
    bool match(string content, bool must_advance);
    bool match(vector<string> content, bool must_advance);
    void printAction(string action, bool newLine, bool printLevel);

    bool program();

    bool declaration();
    bool statement();
    bool assignment();

    bool type();
    bool block();

    bool keyStatement();
    bool ifStatement();
    bool whileStatement();
    bool doWhileStatement();
    bool forStatement();
    bool returnStatement();

    bool expression();
    bool parseBinaryExpression();
    bool parseUnaryExpression();
    bool parsePrimaryExpression();
};

#endif
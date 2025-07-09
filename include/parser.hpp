#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>

#include "generator.hpp"
#include "scope.hpp"
#include "token.hpp"

class Parser {
   public:
    Parser(const std::vector<Token>& tokens, Generator& generator) : tokens(tokens), pos(0), level(0), generator(generator) {};
    void parse();

   private:
    vector<string> TYPES = {"int", "float", "double", "char", "void"};
    vector<string> TYPE_DECORATORS = {"unsigned", "short", "long", "signed"};
    vector<string> OPERATORS_BINARY = {"+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">="};
    vector<string> OPERATORS_UNARY_PREFIX = {"!", "+", "-", "++", "--"};
    vector<string> OPERATORS_UNARY_POSTFIX = {"++", "--"};

    const std::vector<Token>& tokens;
    size_t pos;
    int level = 0;
    Scope scope;
    Generator& generator;

    Token
    peek() const;
    Token advance();
    Token last() const;
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
    bool loopCommandsStatement();

    bool expression();
    bool parseBinaryExpression();
    bool parseUnaryExpression();
    bool parsePrimaryExpression();
};

class ParserException : public exception {
   public:
    ParserException(string message, Token token)
        : message(message), token(token) {
        full_message = message + " '" + token.content + "' (" +
                       to_string(token.position.first) + ", " +
                       to_string(token.position.second) + ")";
    }

    const char* what() const noexcept override {
        return full_message.c_str();
    }

   private:
    string message;
    Token token;
    string full_message;
};

#endif
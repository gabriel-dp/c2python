#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"

class Parser {
   public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0), level(0) {}
    bool parse();

   private:
    const std::vector<Token>& tokens;
    size_t pos;
    int level = 0;

    Token peek() const;
    Token advance();
    bool match(TokenType type, bool must_advance);
    bool match(string content, bool must_advance);
    void printAction(string action);

    bool program();
    bool declaration();
    bool statement();
    bool assignment();
    bool ifStatement();
    bool whileStatement();
    bool block();
    bool expression();
};

#endif
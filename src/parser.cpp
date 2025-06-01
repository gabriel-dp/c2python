#include "parser.hpp"

#include "token.hpp"

using namespace std;

Token Parser::peek() const {
    return pos < tokens.size() ? tokens[pos] : Token{TokenType::END};
}

Token Parser::advance() {
    return pos < tokens.size() ? tokens[pos++] : Token{TokenType::END};
}

bool Parser::match(string content, bool must_advance = true) {
    if (peek().content == content) {
        if (must_advance) advance();
        return true;
    }
    return false;
}

bool Parser::match(vector<string> contents, bool must_advance = true) {
    for (string content : contents) {
        if (peek().content == content) {
            if (must_advance) advance();
            return true;
        }
    }
    return false;
}

bool Parser::match(TokenType type, bool must_advance = true) {
    if (peek().type == type) {
        if (must_advance) advance();
        return true;
    }
    return false;
}

void Parser::printAction(string action, bool newLine = true, bool printLevel = true) {
    if (printLevel) {
        for (int i = 0; i < level; i++) {
            cout << "\t";
        }
    }
    cout << action;
    if (newLine) cout << endl;
}

bool Parser::parse() {
    return program() && peek().type == TokenType::END;
}

bool Parser::program() {
    while (peek().type != TokenType::END) {
        if (!statement()) return false;
    }
    return true;
}

bool Parser::statement() {
    return declaration() || assignment() || keyStatement() || block() || match(";");
}

bool Parser::keyStatement() {
    return ifStatement() || whileStatement() || doWhileStatement() || forStatement() || returnStatement();
}

bool Parser::type() {
    if (match(TYPES)) return true;
    if (match(TYPE_DECORATORS)) return type();
    return false;
}

bool Parser::declaration() {
    if (type()) {
        printAction("DECLARATION", false);
        do {
            if (match("*")) {
                // pointer
            }
            if (match(TokenType::IDENTIFICATOR)) {
                if (match("=") && expression()) {
                    printAction(" ASSIGNMENT", false, false);
                } else if (match("(")) {
                    printAction(" FUNCTION", true, false);
                    while (!match(")") && !match(",")) {
                        if (!(type() && ((match("*") && match(TokenType::IDENTIFICATOR)) || match(TokenType::IDENTIFICATOR)))) {
                            return false;
                        }
                    };
                    level++;
                    if (!statement()) return false;
                    level--;
                    return true;
                }
                if (!match(",") && !match(";", false)) {
                    return false;
                }
            } else {
                return false;
            }
        } while (!match(";"));
        cout << "\n";
        return true;
    }
    return false;
}

bool Parser::assignment() {
    if (match(TokenType::IDENTIFICATOR)) {
        if (match("=")) {
            if (expression() && match(";")) {
                printAction("ASSIGNMENT");
                return true;
            }
        }
    }
    return false;
}

bool Parser::block() {
    if (match("{")) {
        while (!match("}")) {
            if (!statement()) return false;
        }
        return true;
    }
    return false;
}

bool Parser::expression() {
    return parseBinaryExpression();
}

bool Parser::parseBinaryExpression() {
    if (!parseUnaryExpression()) return false;

    while (match(OPERATORS_BINARY)) {
        if (!parseUnaryExpression()) return false;
    }
    return true;
}

bool Parser::parseUnaryExpression() {
    if (match(OPERATORS_UNARY_PREFIX)) {
        return parseUnaryExpression();
    }

    return parsePrimaryExpression();
}

bool Parser::parsePrimaryExpression() {
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFICATOR)) {
        match(OPERATORS_UNARY_POSTFIX);
        return true;
    }

    if (match("(")) {
        if (!expression()) return false;
        return match(")");
    }

    return false;
}

bool Parser::ifStatement() {
    if (match("if")) {
        printAction("IF");
        if (match("(") && expression() && match(")")) {
            level++;
            if (!statement()) return false;
            level--;
            if (match("else")) {
                printAction("ELSE");
                level++;
                if (!statement()) return false;
                level--;
            }
            return true;
        }
    }
    return false;
}

bool Parser::whileStatement() {
    if (match("while")) {
        printAction("WHILE");
        if (match("(") && expression() && match(")")) {
            level++;
            if (!statement()) return false;
            level--;
            return true;
        }
    }
    return false;
}

bool Parser::doWhileStatement() {
    if (match("do")) {
        printAction("DO-WHILE");
        level++;
        if (!statement()) return false;
        level--;
        return match("while") && match("(") && expression() && match(")") && match(";");
    }
    return false;
}

bool Parser::forStatement() {
    if (match("for")) {
        printAction("FOR", false);
        if (match("(") && (declaration() || assignment() || (expression() && match(";")) || match(";")) && ((expression() && match(";")) || match(";")) && ((expression() && match(")")) || match(")"))) {
            level++;
            if (!statement()) return false;
            level--;
            return true;
        }
    }
    return false;
}

bool Parser::returnStatement() {
    if (match("return")) {
        printAction("RETURN");
        return expression();
    }
    return false;
}

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

bool Parser::match(TokenType type, bool must_advance = true) {
    if (peek().type == type) {
        if (must_advance) advance();
        return true;
    }
    return false;
}

void Parser::printAction(string action) {
    for (int i = 0; i < level; i++) {
        cout << "\t";
    }
    cout << action << endl;
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
    bool result = declaration() || assignment() || ifStatement() || whileStatement() || block() || match(";");
    return result;
}

bool Parser::declaration() {
    if (match("int") || match("float")) {
        do {
            if (match(TokenType::IDENTIFICATOR)) {
                if (!match(",") && !match(";", false)) {
                    return false;
                }
            } else {
                return false;
            }
        } while (!match(";"));
        printAction("VARIABLE");
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

bool Parser::ifStatement() {
    if (match("if")) {
        if (match("(") && expression() && match(")")) {
            printAction("CONDITION - IF");
            if (!statement()) return false;
            if (match("else")) {
                printAction("CONDITION - ELSE");
                return statement();
            }
            return true;
        }
    }
    return false;
}

bool Parser::whileStatement() {
    if (match("while")) {
        if (match("(") && expression() && match(")")) {
            printAction("LOOP - WHILE");
            return statement();
        }
    }
    return false;
}

bool Parser::block() {
    if (match("{")) {
        level++;
        while (!match("}")) {
            if (!statement()) return false;
        }
        level--;
        return true;
    }
    return false;
}

bool Parser::expression() {
    // Just a very simplified expression parser (recursive)
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFICATOR)) {
        if (match("+") || match("-")) {
            return expression();
        }
        return true;
    } else if (match("(")) {
        if (!expression()) return false;
        return match(")");
    }
    return false;
}

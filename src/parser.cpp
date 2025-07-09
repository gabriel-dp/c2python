#include "parser.hpp"

#include "token.hpp"

using namespace std;

Token Parser::peek() const {
    return pos < tokens.size() ? tokens[pos] : Token{TokenType::END};
}

Token Parser::advance() {
    return pos < tokens.size() ? tokens[pos++] : Token{TokenType::END};
}

Token Parser::last() const {
    return pos > 0 ? tokens[pos - 1] : Token{TokenType::END};
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
    return ifStatement() || whileStatement() || doWhileStatement() || forStatement() || returnStatement() || loopCommandsStatement();
}

bool Parser::type() {
    if (match(TYPES)) return true;
    if (match(TYPE_DECORATORS)) return type();
    return false;
}

bool Parser::declaration() {
    if (type()) {
        string variable_type = last().content;
        printAction("DECLARATION", false);
        do {
            if (match("*")) {
                // pointer
            }
            if (match(TokenType::IDENTIFICATOR)) {
                string variable_identificator = last().content;
                if (scope.get_variable(variable_identificator) != nullptr) {
                    // already declared
                    return false;
                }

                if (match("=") && expression()) {
                    scope.insert_variable_data(variable_identificator, VariableData(variable_type, true));
                    printAction(" ASSIGNMENT", false, false);
                } else if (match("(")) {
                    if (scope.actual_context_data().is_function) {
                        // function inside function
                        return false;
                    }
                    scope.insert_variable_data(variable_identificator, VariableData(variable_type, true));
                    printAction(" FUNCTION", true, false);
                    while (!match(")") && !match(",")) {
                        if (!(type() && ((match("*") && match(TokenType::IDENTIFICATOR)) || match(TokenType::IDENTIFICATOR)))) {
                            return false;
                        }
                    };
                    scope.actual_context_data().is_function = true;
                    if (!statement()) return false;
                    scope.actual_context_data().is_function = false;
                    return true;
                } else {
                    scope.insert_variable_data(variable_identificator, VariableData(variable_type));
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
        VariableData* variable = scope.get_variable(last().content);
        if (variable == nullptr) {
            // not declared
            return false;
        }

        if (match("=")) {
            if (expression() && match(";")) {
                printAction("ASSIGNMENT");
                variable->is_used = true;
                return true;
            }
        }
    }
    return false;
}

bool Parser::block() {
    if (match("{")) {
        scope.init();
        while (!match("}")) {
            if (!statement()) return false;
        }
        scope.endActual();
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
        if (last().type == TokenType::IDENTIFICATOR) {
            VariableData* variable = scope.get_variable(last().content);
            if (variable == nullptr) {
                // not declared
                return false;
            }
            if (!variable->is_initialized) {
                // not initialized
                return false;
            }
            variable->is_used = true;
        }
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
            if (!statement()) return false;
            if (match("else")) {
                printAction("ELSE");
                if (!statement()) return false;
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
            if (!statement()) return false;
            return true;
        }
    }
    return false;
}

bool Parser::doWhileStatement() {
    if (match("do")) {
        printAction("DO-WHILE");
        if (!statement()) return false;
        return match("while") && match("(") && expression() && match(")") && match(";");
    }
    return false;
}

bool Parser::forStatement() {
    if (match("for")) {
        printAction("FOR", false);
        if (match("(") && (declaration() || assignment() || (expression() && match(";")) || match(";")) && ((expression() && match(";")) || match(";")) && ((expression() && match(")")) || match(")"))) {
            if (!statement()) return false;
            return true;
        }
    }
    return false;
}

bool Parser::returnStatement() {
    if (match("return")) {
        if (!scope.actual_context_data().is_function) {
            // invalid return
            return false;
        }
        printAction("RETURN");
        return expression();
    }
    return false;
}

bool Parser::loopCommandsStatement() {
    if (match("break") || match("continue")) {
        if (!scope.actual_context_data().is_loop) {
            // invalid loop command
            return false;
        }
        return true;
    }
    return false;
}

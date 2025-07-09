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

void Parser::parse() {
    if (program() && peek().type == TokenType::END) {
        cout << "Parsing successful!\n";
    } else {
        cout << "Parsing failed.\n";
    }
}

bool Parser::program() {
    try {
        while (peek().type != TokenType::END) {
            statement();
        }
    } catch (const ParserException& e) {
        cout << "\n"
             << e.what()
             << "\n";
        return false;
    }
    return true;
}

bool Parser::statement() {
    generator.startStatement();
    bool result = declaration() || assignment() || keyStatement() || block() || match(";");
    generator.endStatement();
    if (!result) throw ParserException("Invalid statement", peek());
    return result;
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
                    throw ParserException("Redefinition of variable", last());
                }

                if (match("=")) {
                    generator.generateAssignment(variable_identificator);
                    if (expression()) {
                        scope.insert_variable_data(variable_identificator, VariableData(variable_type, true));
                        printAction(" ASSIGNMENT", false, false);
                    }
                } else if (match("(")) {
                    if (scope.actual_context_data().is_function) {
                        throw ParserException("Nested functions are not allowed", last());
                    }
                    generator.generateFunction(variable_identificator);
                    scope.insert_variable_data(variable_identificator, VariableData(variable_type, true));
                    printAction(" FUNCTION", true, false);
                    while (!match(")") && !match(",")) {
                        if (!(type() && ((match("*") && match(TokenType::IDENTIFICATOR)) || match(TokenType::IDENTIFICATOR)))) {
                            return false;
                        }
                    };
                    scope.actual_context_data().is_function = true;
                    statement();
                    scope.actual_context_data().is_function = false;
                    return true;
                } else {
                    generator.generateDeclaration(variable_identificator);
                    scope.insert_variable_data(variable_identificator, VariableData(variable_type));
                }

                if (!match(",")) {
                    if (!match(";", false)) {
                        return false;
                    }
                } else {
                    generator.endStatement();
                    generator.startStatement();
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
        string identificator = last().content;
        VariableData* variable = scope.get_variable(identificator);
        if (variable == nullptr) {
            throw ParserException("Undeclared variable", last());
        }

        if (match("=")) {
            generator.generateAssignment(identificator);
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
        generator.startBlock();
        while (!match("}")) {
            statement();
        }
        generator.endBlock();
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
        generator.generateToken(last().content);
        if (!parseUnaryExpression()) return false;
    }
    return true;
}

bool Parser::parseUnaryExpression() {
    if (match(OPERATORS_UNARY_PREFIX)) {
        generator.generateToken(last().content);
        return parseUnaryExpression();
    }

    return parsePrimaryExpression();
}

bool Parser::parsePrimaryExpression() {
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFICATOR)) {
        if (last().type == TokenType::IDENTIFICATOR) {
            VariableData* variable = scope.get_variable(last().content);
            if (variable == nullptr) {
                throw ParserException("Undeclared variable", last());
            }
            if (!variable->is_initialized) {
                throw ParserException("Uninitialized variable", last());
            }
            variable->is_used = true;
        }
        generator.generateToken(last().content);
        if (match(OPERATORS_UNARY_POSTFIX)) {
            generator.generateToken(last().content);
        }
        return true;
    }

    if (match("(")) {
        generator.generateToken(last().content);
        if (!expression()) return false;
        generator.generateToken(peek().content);
        if (!match(")")) {
            throw ParserException("Expected close bracket", peek());
        }
        return true;
    }

    return false;
}

bool Parser::ifStatement() {
    if (match("if")) {
        printAction("IF");
        generator.generateCondition();
        if (match("(") && expression() && match(")")) {
            statement();
            if (match("else")) {
                generator.startStatement();
                generator.endBlock();
                generator.generateConditionNot();
                generator.startBlock();
                generator.startStatement();
                printAction("ELSE");
                statement();
            }
            return true;
        }
    }
    return false;
}

bool Parser::whileStatement() {
    if (match("while")) {
        generator.generateLoop();
        printAction("WHILE");
        if (match("(") && expression() && match(")")) {
            statement();
            return true;
        }
    }
    return false;
}

bool Parser::doWhileStatement() {
    if (match("do")) {
        printAction("DO-WHILE");
        statement();
        if (match("while") && match("(") && expression() && match(")") && match(";")) {
            return true;
        }
    }
    return false;
}

bool Parser::forStatement() {
    if (match("for")) {
        printAction("FOR", false);
        scope.init();
        if (match("(") && (declaration() || assignment() || (expression() && match(";")) || match(";")) && ((expression() && match(";")) || match(";")) && ((expression() && match(")")) || match(")"))) {
            statement();
            scope.endActual();
            return true;
        }
    }
    return false;
}

bool Parser::returnStatement() {
    if (match("return")) {
        generator.generateReturn();
        if (!scope.actual_context_data().is_function) {
            throw ParserException("Return outside of a function", last());
        }
        printAction("RETURN");
        return expression();
    }
    return false;
}

bool Parser::loopCommandsStatement() {
    if (match("break") || match("continue")) {
        if (!scope.actual_context_data().is_loop) {
            throw ParserException("Not inside a loop", last());
        }
        return true;
    }
    return false;
}
